#pragma once
#include<chrono>
#include<thread>
#include<vector>
#include<functional>

#define GROOVE_NUM 512
#define TIME_STEP 10

namespace timerUtil
{
	struct TaskInfo
	{
		int turnIndex;						// 任务所在的轮数
		std::function<void()> func;			// 任务执行函数
	};

	// 时间轮
	class TimeWheel
	{
	public:
		using TaskList = std::vector<TaskInfo>;

		uint32_t grooveNum;					// 每一轮的槽数
		uint32_t currentTurns;				// 当前的轮数
		int timeStep;						// 轮循的时间步长，单位毫秒ms

		std::vector<TaskList> wheel;

		std::unique_ptr<std::thread> loopThread;		// 时间轮循线程

		TimeWheel():grooveNum(GROOVE_NUM), timeStep(TIME_STEP), currentTurns(0)
		{
			wheel.reserve(grooveNum);
			start();
		}

		~TimeWheel()
		{
			wheel.clear();
			currentTurns = 0;
			loopThread->join();
		}

	private:
		bool isRunning = false;

		void start()
		{
			// 开启线程的主循环
			loopThread = std::make_unique<std::thread>(std::bind(&TimeWheel::mainLoop, this));
		}

		void mainLoop()
		{
			while (isRunning)
			{

			}
		}

		// 时间向前走
		void timeForward()
		{
			
		}
	};

	class Timer
	{
		//std::chrono::milliseconds

	};
}