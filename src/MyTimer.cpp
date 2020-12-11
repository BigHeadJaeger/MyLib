#include "MyTimer.hpp"

using namespace Timer;

Timer::LayerTimeWheel::LayerTimeWheel()
{
	wheels.reserve(LAYER_NUM);
	int lastTotalTimes = TIME_STEP;
	// 构造LAYER_NUM个时间轮的指针间隔
	for (int i = 0; i < LAYER_NUM; i++)
	{
		Wheel wheel;
		wheel.setGrooveNum(GROOVE_NUM);
		wheel.timeStep = lastTotalTimes;
		lastTotalTimes = wheel.grooveNum * wheel.timeStep;
		wheels.push_back(wheel);
	}

	start();
}

uint32_t Timer::LayerTimeWheel::addNewTask(std::function<void()> func, std::chrono::milliseconds interval, bool isLoop)
{
	auto task = std::make_shared<TaskInfo>(func, interval, isLoop);
	task->id = ids++;
	taskMap[task->id] = task;
	insertTask(task);
	return task->id;
}

void Timer::LayerTimeWheel::removeTaskByID(uint32_t id)
{
	auto mapIt = taskMap.find(id);
	if (mapIt != taskMap.end())
	{
		if (!mapIt->second.expired())
		{
			int layerIndex = mapIt->second.lock()->layerIndex;
			int pos = mapIt->second.lock()->posInfo[layerIndex];
			auto& targetList = wheels[layerIndex].taskWheel[pos];
			for (auto it = targetList.begin(); it != targetList.end(); it++)
			{
				if ((*it)->id == id)
				{// 此处不删除任务，删除统一在循环中进行，否则会导致主循环的迭代器失效
					// 关闭任务
					(*it)->isActive = false;
					break;
				}
			}
			// 删除map中的记录
			taskMap.erase(mapIt);
		}
		else
			std::cout << "error task expired" << std::endl;
	}
}

void Timer::LayerTimeWheel::start()
{
	isRunning = true;
	// 开启线程的主循环
	loopThread = std::make_unique<std::thread>(std::bind(&LayerTimeWheel::mainLoop, this));
}

void Timer::LayerTimeWheel::stop()
{
	isRunning = false;
	loopThread->join();
}

void Timer::LayerTimeWheel::mainLoop()
{
	while (isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(TIME_STEP));	// 每次循环睡眠指针移动一格的时间
		timeForward();
	}
}

void Timer::LayerTimeWheel::timeForward()
{
	bool isAdd = true;
	for (size_t i = 0; i < wheels.size(); i++)
	{
		if (isAdd)
		{
			isAdd = wheels[i].tickForward();
			if (isAdd && i == (wheels.size() - 1))
			{// 一般不会超过，自时间轮开启可以走102年
				std::cout << "Exceeded the maximum time limit" << std::endl;
				return;
			}
			processOneWheelTask(wheels[i], i);
		}
	}
}

void Timer::LayerTimeWheel::processOneWheelTask(Wheel& wheel, int index)
{
	int tick = wheel.tick;
	auto& list = wheel.taskWheel[tick];
	auto it = list.begin();
	for (it; it != list.end(); it)
	{
		auto taskInfo = (*it);
		// 从当前列表删除任务
		it = list.erase(it);

		if (!taskInfo->isActive)
		{
			continue;
		}

		if (index == 0)
		{// 底层时间轮直接执行
			taskInfo->func();
			// 根据是否循环重新插入任务
			if (taskInfo->isLoop)
				insertTask(std::move(taskInfo));
			else
				removeTaskFromMap(taskInfo->id);
		}
		else
		{
			// 先看位置信息当前层之下是否还有移动量
			int targetLayer = index;
			for (int i = index - 1; i >= 0; i--)
			{
				if (taskInfo->posInfo[i] >= 1)
				{
					targetLayer = i;
					break;
				}
			}
			if (targetLayer == index)
			{// 如果下层没有移动量则执行任务
				taskInfo->func();
				if (taskInfo->isLoop)
					insertTask(std::move(taskInfo));
				else
					removeTaskFromMap(taskInfo->id);
			}
			else
			{
				// 刷新任务的位置信息
				taskInfo->posInfo[index] = 0;
				insertToTargetPos(taskInfo, index - 1, taskInfo->posInfo[index - 1]);
			}
		}
	}
}

void Timer::LayerTimeWheel::removeTaskFromMap(uint32_t id)
{
	auto it = taskMap.find(id);
	if (it != taskMap.end())
		taskMap.erase(it);
}

Wheel& Timer::LayerTimeWheel::getLowerWheel(int curIndex)
{
	if (curIndex == 0)
		return wheels[curIndex];
	else
		return wheels[curIndex - 1];
}

Wheel& Timer::LayerTimeWheel::getUpperWheel(int curIndex)
{
	if (curIndex == LAYER_NUM)
		return wheels[curIndex];
	else
		return wheels[curIndex + 1];
}

void Timer::LayerTimeWheel::insertTask(std::shared_ptr<TaskInfo> taskInfo)
{
	int moveStep = taskInfo->interval.count() / wheels[0].timeStep;
	int add = moveStep;
	int remain = 0;
	int resLayer = 0;

	// 从底层循环计算每一层指针的移动次数
	for (int i = 0; i < LAYER_NUM; i++)
	{
		int lastAdd = add;
		add = (lastAdd + wheels[i].tick) / wheels[i].grooveNum;
		remain = (lastAdd + wheels[i].tick) % wheels[i].grooveNum;
		taskInfo->posInfo[i] = remain;
		if ((add == 1 && remain == 0) || (add == 0))
		{
			resLayer = i;
			break;
		}
	}
	taskInfo->layerIndex = resLayer;
	wheels[resLayer].taskWheel[remain].push_back(taskInfo);

}

void Timer::LayerTimeWheel::insertToTargetPos(std::shared_ptr<TaskInfo> taskInfo, int wheelIndex, int groovePos)
{
	wheels[wheelIndex].taskWheel[groovePos].push_back(taskInfo);
}
