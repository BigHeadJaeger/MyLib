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
		int turnIndex;						// �������ڵ�����
		std::function<void()> func;			// ����ִ�к���
	};

	// ʱ����
	class TimeWheel
	{
	public:
		using TaskList = std::vector<TaskInfo>;

		uint32_t grooveNum;					// ÿһ�ֵĲ���
		uint32_t currentTurns;				// ��ǰ������
		int timeStep;						// ��ѭ��ʱ�䲽������λ����ms

		std::vector<TaskList> wheel;

		std::unique_ptr<std::thread> loopThread;		// ʱ����ѭ�߳�

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
			// �����̵߳���ѭ��
			loopThread = std::make_unique<std::thread>(std::bind(&TimeWheel::mainLoop, this));
		}

		void mainLoop()
		{
			while (isRunning)
			{

			}
		}

		// ʱ����ǰ��
		void timeForward()
		{
			
		}
	};

	class Timer
	{
		//std::chrono::milliseconds

	};
}