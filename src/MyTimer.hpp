#pragma once
#include<chrono>
#include<thread>
#include<vector>
#include<functional>
#include<list>
#include<atomic>
#include<map>
#include<iostream>

//#define GROOVE_NUM 512
//#define TIME_STEP 10				// 时间步长

#define GROOVE_NUM 5				// 每层的槽数
#define TIME_STEP 10				// 时间步长
#define LAYER_NUM 5					// 时间轮的层数

namespace Timer
{
	// 任务间隔需要转换成每层对应的指针走动数
	// <0, 3> 第一层走3格
	// <1, 1> 第二层走1格
	// <2, 0> 第三层走0格
	// <3, 0> 第四层走0格
	// <4, 0> 第五层走0格
	// 当每一层的指针走到了当前任务则清空
	struct TaskInfo
	{
		uint32_t id;

		bool isLoop;
		std::chrono::milliseconds interval;
		int layerIndex;						// 任务所在的层数
		std::function<void()> func;			// 任务执行函数

		std::map<short, int> posInfo;		// 任务插入的时候每一层走过的格数

		bool isActive = true;

		TaskInfo(std::function<void()> _func, std::chrono::milliseconds _interval, bool _isLoop)
		{
			func = _func;
			interval = _interval;
			isLoop = _isLoop;
		}
	};

	// 时间轮
	using TaskList = std::list<std::shared_ptr<TaskInfo>>;

	// 一层时间轮的信息
	class Wheel
	{
	public:
		uint32_t grooveNum;					// 每一轮的槽数
		int timeStep;						// 轮循的时间步长，单位毫秒ms
		int tick;							// 时间轮的指针（每次移动一个槽位）

		std::vector<TaskList> taskWheel;	// 时间轮的任务管理

	public:
		Wheel()
		{
			tick = 0;
		}

		void setGrooveNum(int num)
		{
			grooveNum = num;
			taskWheel.resize(num, TaskList());
		}
		void setTimeStep(int num)
		{
			timeStep = num;
		}

		// 指针向前移动
		bool tickForward()
		{
			bool isAdding = false;		// 是否进位
			tick++;
			if (tick >= grooveNum)
			{
				isAdding = true;
				tick = 0;
			}
			return isAdding;
		}
	};

	// 分层时间轮
	// 每层200个槽，第一层的指针跨度是10ms，之后每一层的指针跨度是上一层的总跨度
	// 5层可表示的时间大约102年
	// 每一个时间轮的刻度1代表开始，刻度0代表末尾（10ms的任务在第一轮的1刻度， 200ms的任务在第一轮的0刻度， 210ms在第二轮的1刻度）
	class LayerTimeWheel
	{
	private:
		std::vector<Wheel> wheels;		// 每一层的时间轮集合
		bool isRunning = false;
		std::shared_ptr<std::thread> loopThread;		// 时间轮循线程

		std::atomic_uint ids = 1;

		std::map<uint32_t, std::weak_ptr<TaskInfo>> taskMap;	// 通过id查询到任务
	public:
		LayerTimeWheel()
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

		static LayerTimeWheel& getInstance()
		{
			static LayerTimeWheel instance;
			return instance;
		}

		uint32_t addNewTask(std::function<void()> func, std::chrono::milliseconds interval, bool isLoop = true)
		{
			auto task = std::make_shared<TaskInfo>(func, interval, isLoop);
			task->id = ids++;
			taskMap[task->id] = task;
			insertTask(task);
			return task->id;
		}

		void removeTaskByID(uint32_t id)
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
	private:
		void start()
		{
			isRunning = true;
			// 开启线程的主循环
			loopThread = std::make_unique<std::thread>(std::bind(&LayerTimeWheel::mainLoop, this));
		}

		void stop()
		{
			isRunning = false;
			loopThread->join();
		}

		void mainLoop()
		{
			while (isRunning)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(TIME_STEP));	// 每次循环睡眠指针移动一格的时间
				timeForward();
				//processTaskList();
			}
		}

		// 时间向前走一个单位，从底到高层顺序遍历每一层的时间轮，前一层发生进位下一层指针移动一次
		// 对于每一层的时间轮，只要指针发生移动则需要处理新指针位置下的任务
		void timeForward()
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

		// 除了最底层的时间轮都需要将任务降级处理
		void processOneWheelTask(Wheel& wheel, int index)
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
			//wheel.taskWheel[tick]
		}

		void removeTaskFromMap(uint32_t id)
		{
			auto it = taskMap.find(id);
			if (it != taskMap.end())
				taskMap.erase(it);
		}

		// 获取当前轮的下一层
		Wheel& getLowerWheel(int curIndex)
		{
			if (curIndex == 0)
				return wheels[curIndex];
			else
				return wheels[curIndex - 1];
		}

		Wheel& getUpperWheel(int curIndex)
		{
			if (curIndex == LAYER_NUM)
				return wheels[curIndex];
			else
				return wheels[curIndex + 1];
		}

		// 添加任务，根据interval计算出
		void insertTask(std::shared_ptr<TaskInfo> taskInfo)
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

		void insertToTargetPos(std::shared_ptr<TaskInfo> taskInfo, int wheelIndex, int groovePos)
		{
			wheels[wheelIndex].taskWheel[groovePos].push_back(taskInfo);
		}

	};

	uint32_t schedule(std::function<void()> func, int seconds)
	{
		std::chrono::milliseconds t(seconds * 1000);
		auto id = LayerTimeWheel::getInstance().addNewTask(func, t, true);
		return id;
	}

	uint32_t scheduleOnce(std::function<void()> func, int seconds)
	{
		std::chrono::milliseconds t(seconds * 1000);
		auto id = LayerTimeWheel::getInstance().addNewTask(func, t, false);
		return id;
	}

	void unSchedule(uint32_t id)
	{
		LayerTimeWheel::getInstance().removeTaskByID(id);
	}
}