#pragma once
#include<chrono>
#include<thread>
#include<vector>
#include<functional>
#include<list>
#include<atomic>
#include<map>

#define GROOVE_NUM 512
#define TIME_STEP 10				// 时间步长

namespace Timer
{
	struct TaskInfo
	{
		uint32_t id;
		bool isLoop;
		std::chrono::milliseconds interval;
		int turnIndex;						// 任务所在的轮数（处理每一个槽的时候只处理turnIndex和currentTurns相等的任务）
		int grovePos;						// 所在槽的位置
		std::function<void()> func;			// 任务执行函数

		TaskInfo(std::function<void()> _func, std::chrono::milliseconds _interval, bool _isLoop)
		{
			func = _func;
			interval = _interval;
			isLoop = _isLoop;
		}
	};

	// 时间轮
	using TaskList = std::list<std::shared_ptr<TaskInfo>>;
	class TimeWheel
	{
	private:
		uint32_t grooveNum;					// 每一轮的槽数
		uint32_t currentTurns;				// 当前的轮数
		int timeStep;						// 轮循的时间步长，单位毫秒ms

		int tick;							// 时间轮的指针（每次移动一个槽位）

		std::atomic_uint ids = 1;				// 每开启一个计时器分配一个id

		std::vector<TaskList> taskWheel;

		std::shared_ptr<std::thread> loopThread;		// 时间轮循线程

		std::map<uint32_t, int> taskManager;		// 通过id找到某个任务（用于查找<id, grovePos>）

	public:
		TimeWheel() :grooveNum(GROOVE_NUM), timeStep(TIME_STEP), currentTurns(0), tick(0)
		{
			taskWheel.resize(grooveNum, TaskList());
			start();
		}

		~TimeWheel()
		{
			taskWheel.clear();
			currentTurns = 0;
			stop();
		}
		TimeWheel(const TimeWheel&) = delete;

		static TimeWheel& getInstance()
		{
			static TimeWheel instance;
			return instance;
		}
	private:
		bool isRunning = false;

		void start()
		{
			isRunning = true;
			// 开启线程的主循环
			loopThread = std::make_unique<std::thread>(std::bind(&TimeWheel::mainLoop, this));
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
				std::this_thread::sleep_for(std::chrono::milliseconds(timeStep));	// 每次循环睡眠指针移动一格的时间
				tickForward();
				processTaskList();
			}
		}

		// 时针向前走
		void tickForward()
		{
			tick++;
			auto n1 = tick / grooveNum;
			if (n1 >= 1)
			{// 进入下一轮
				auto remainder = tick % grooveNum;
				tick = remainder;
				currentTurns++;
			}
		}

		// 处理当前时针下的任务列表
		void processTaskList()
		{
			TaskList test;
			auto& taskList = taskWheel[tick];

			auto it = taskList.begin();
			for (it; it != taskList.end();)
			{
				if ((*it)->turnIndex == currentTurns)
				{
					auto pTask = (*it);
					// 将任务从list中移除
					it = taskList.erase(it);

					// 执行定时方法
					pTask->func();
					// 根据是否循环重新放置任务位置
					if (pTask->isLoop)
					{
						insertTask(pTask);
					}
				}
				else
					it++;
			}
		}

		// 添加任务
		void insertTask(std::shared_ptr<TaskInfo>& taskInfo)
		{
			taskInfo->interval;

			auto moveStep = taskInfo->interval.count() / timeStep;		// 经过此时间间隔指针需要走的次数
			auto totalTick = tick + moveStep;

			int resTurns = totalTick / grooveNum + currentTurns;
			int resTick = totalTick % grooveNum;

			taskInfo->turnIndex = resTurns;
			taskInfo->grovePos = resTick;

			taskManager[taskInfo->id] = resTick;	// 记录任务在第几个槽

			taskWheel[resTick].push_back(taskInfo);
		}

		//// 添加任务接口
		//void insertTask(std::function<void>& func, std::chrono::milliseconds interval, bool isLoop = true)
		//{
		//	auto taskInfo = std::make_shared<TaskInfo>(func, interval, isLoop);
		//	// 根据间隔计算需要放在哪一层的哪一个槽 	


		//}

	public:
		uint32_t addNewTask(std::function<void()> func, std::chrono::milliseconds interval, bool isLoop = true)
		{
			auto task = std::make_shared<TaskInfo>(func, interval, isLoop);
			task->id = ids++;
			insertTask(task);
			return task->id;
		}
	};


	uint32_t schedule(std::function<void()> func, int seconds)
	{
		std::chrono::milliseconds t(seconds * 1000);
		auto id = TimeWheel::getInstance().addNewTask(func, t, true);
		return id;
	}

	uint32_t scheduleOnce(std::function<void()> func, int seconds)
	{
		std::chrono::milliseconds t(seconds * 1000);
		auto id = TimeWheel::getInstance().addNewTask(func, t, false);
		return id;
	}
}