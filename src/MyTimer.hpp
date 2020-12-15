#pragma once
#include<chrono>
#include<thread>
#include<vector>
#include<functional>
#include<list>
#include<atomic>
#include<map>
#include<iostream>

#define GROOVE_NUM 200				// 每层的槽数
#define TIME_STEP 10				// 时间步长(ms)
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
		LayerTimeWheel();
		static LayerTimeWheel& getInstance()
		{
			static LayerTimeWheel instance;
			return instance;
		}

		uint32_t addNewTask(std::function<void()> func, std::chrono::milliseconds interval, bool isLoop = true);
		void removeTaskByID(uint32_t id);
	private:
		void start();
		void stop();
		void mainLoop();

		// 时间向前走一个单位，从底到高层顺序遍历每一层的时间轮，前一层发生进位下一层指针移动一次
		// 对于每一层的时间轮，只要指针发生移动则需要处理新指针位置下的任务
		void timeForward();

		// 除了最底层的时间轮都需要将任务降级处理
		void processOneWheelTask(Wheel& wheel, int index);
		void removeTaskFromMap(uint32_t id);

		// 获取当前轮的下一层
		Wheel& getLowerWheel(int curIndex);
		Wheel& getUpperWheel(int curIndex);

		// 添加任务，根据interval计算出
		void insertTask(std::shared_ptr<TaskInfo> taskInfo);
		void insertToTargetPos(std::shared_ptr<TaskInfo> taskInfo, int wheelIndex, int groovePos);
	};



	// 对外接口
	uint32_t schedule(std::function<void()> func, float seconds);
	uint32_t scheduleOnce(std::function<void()> func, float seconds);
	void unSchedule(uint32_t id);
}