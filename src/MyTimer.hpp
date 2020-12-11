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
	//struct TaskInfo
	//{
	//	uint32_t id;
	//	bool isLoop;
	//	std::chrono::milliseconds interval;
	//	int turnIndex;						// 任务所在的轮数（处理每一个槽的时候只处理turnIndex和currentTurns相等的任务）
	//	int grovePos;						// 所在槽的位置
	//	std::function<void()> func;			// 任务执行函数

	//	TaskInfo(std::function<void()> _func, std::chrono::milliseconds _interval, bool _isLoop)
	//	{
	//		func = _func;
	//		interval = _interval;
	//		isLoop = _isLoop;
	//	}
	//};

	//// 时间轮
	//using TaskList = std::list<std::shared_ptr<TaskInfo>>;
	//class TimeWheel
	//{
	//private:
	//	uint32_t grooveNum;					// 每一轮的槽数
	//	uint32_t currentTurns;				// 当前的轮数
	//	int timeStep;						// 轮循的时间步长，单位毫秒ms

	//	int tick;							// 时间轮的指针（每次移动一个槽位）

	//	std::atomic_uint ids = 1;				// 每开启一个计时器分配一个id

	//	std::vector<TaskList> taskWheel;

	//	std::shared_ptr<std::thread> loopThread;		// 时间轮循线程

	//	std::map<uint32_t, int> taskManager;		// 通过id找到某个任务（用于查找<id, grovePos>）

	//public:
	//	TimeWheel() :grooveNum(GROOVE_NUM), timeStep(TIME_STEP), currentTurns(0), tick(0)
	//	{
	//		taskWheel.resize(grooveNum, TaskList());
	//		start();
	//	}

	//	~TimeWheel()
	//	{
	//		taskWheel.clear();
	//		currentTurns = 0;
	//		stop();
	//	}
	//	TimeWheel(const TimeWheel&) = delete;

	//	static TimeWheel& getInstance()
	//	{
	//		static TimeWheel instance;
	//		return instance;
	//	}
	//private:
	//	bool isRunning = false;

	//	void start()
	//	{
	//		isRunning = true;
	//		// 开启线程的主循环
	//		loopThread = std::make_unique<std::thread>(std::bind(&TimeWheel::mainLoop, this));
	//	}

	//	void stop()
	//	{
	//		isRunning = false;
	//		loopThread->join();
	//	}

	//	void mainLoop()
	//	{
	//		while (isRunning)
	//		{
	//			std::this_thread::sleep_for(std::chrono::milliseconds(timeStep));	// 每次循环睡眠指针移动一格的时间
	//			tickForward();
	//			processTaskList();
	//		}
	//	}

	//	// 时针向前走
	//	void tickForward()
	//	{
	//		tick++;
	//		auto n1 = tick / grooveNum;
	//		if (n1 >= 1)
	//		{// 进入下一轮
	//			auto remainder = tick % grooveNum;
	//			tick = remainder;
	//			currentTurns++;
	//		}
	//	}

	//	// 处理当前时针下的任务列表
	//	void processTaskList()
	//	{
	//		TaskList test;
	//		auto& taskList = taskWheel[tick];

	//		auto it = taskList.begin();
	//		for (it; it != taskList.end();)
	//		{
	//			if ((*it)->turnIndex == currentTurns)
	//			{
	//				auto pTask = (*it);
	//				// 将任务从list中移除
	//				it = taskList.erase(it);
	//				// 从id记录map中移除

	//				// 执行定时方法
	//				pTask->func();
	//				// 根据是否循环重新放置任务位置
	//				if (pTask->isLoop)
	//				{
	//					insertTask(pTask);
	//				}
	//			}
	//			else
	//				it++;
	//		}
	//	}

	//	// 添加任务
	//	void insertTask(std::shared_ptr<TaskInfo>& taskInfo)
	//	{
	//		taskInfo->interval;

	//		auto moveStep = taskInfo->interval.count() / timeStep;		// 经过此时间间隔指针需要走的次数
	//		auto totalTick = tick + moveStep;

	//		int resTurns = totalTick / grooveNum + currentTurns;
	//		int resTick = totalTick % grooveNum;

	//		taskInfo->turnIndex = resTurns;
	//		taskInfo->grovePos = resTick;

	//		taskManager[taskInfo->id] = resTick;	// 记录任务在第几个槽

	//		taskWheel[resTick].push_back(taskInfo);
	//	}

	//public:
	//	uint32_t addNewTask(std::function<void()> func, std::chrono::milliseconds interval, bool isLoop = true)
	//	{
	//		auto task = std::make_shared<TaskInfo>(func, interval, isLoop);
	//		task->id = ids++;
	//		insertTask(task);
	//		return task->id;
	//	}

	//	// 根据id删除某个任务
	//	void removeTaskById(uint32_t id)
	//	{

	//	}
	//};


	//uint32_t schedule(std::function<void()> func, int seconds)
	//{
	//	std::chrono::milliseconds t(seconds * 1000);
	//	auto id = TimeWheel::getInstance().addNewTask(func, t, true);
	//	return id;
	//}

	//uint32_t scheduleOnce(std::function<void()> func, int seconds)
	//{
	//	std::chrono::milliseconds t(seconds * 1000);
	//	auto id = TimeWheel::getInstance().addNewTask(func, t, false);
	//	return id;
	//}

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
		uint32_t startTime;					// 任务加入时的时间

		bool isLoop;
		std::chrono::milliseconds interval;
		int layerIndex;						// 任务所在的层数
		int groovePos;						// 所在槽的位置
		std::function<void()> func;			// 任务执行函数

		std::map<short, int> posInfo;		// 任务插入的时候每一层走过的格数

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

		static LayerTimeWheel& getInstance()
		{
			static LayerTimeWheel instance;
			return instance;
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

				if (index == 0)
				{// 底层时间轮直接执行
					taskInfo->func();
					// 根据是否循环重新插入任务
					if (taskInfo->isLoop)
						insertTask(std::move(taskInfo));
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
			wheels[resLayer].taskWheel[remain].push_back(taskInfo);
		}

		void insertToTargetPos(std::shared_ptr<TaskInfo> taskInfo, int wheelIndex, int groovePos)
		{
			wheels[wheelIndex].taskWheel[groovePos].push_back(taskInfo);
		}

		void addNewTask(std::function<void()> func, std::chrono::milliseconds interval, bool isLoop = true)
		{
			auto task = std::make_shared<TaskInfo>(func, interval, isLoop);
			insertTask(task);
		}
	};


	uint32_t schedule(std::function<void()> func, int seconds)
	{
		std::chrono::milliseconds t(seconds * 1000);
		LayerTimeWheel::getInstance().addNewTask(func, t, true);
		return 1;
	}

	uint32_t scheduleOnce(std::function<void()> func, int seconds)
	{
		std::chrono::milliseconds t(seconds * 1000);
		LayerTimeWheel::getInstance().addNewTask(func, t, false);
		return 1;
	}
}