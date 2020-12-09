#pragma once
#include<chrono>
#include<thread>
#include<vector>
#include<functional>
#include<list>
#include<atomic>
#include<map>

#define GROOVE_NUM 512
#define TIME_STEP 10				// ʱ�䲽��

namespace Timer
{
	struct TaskInfo
	{
		uint32_t id;
		bool isLoop;
		std::chrono::milliseconds interval;
		int turnIndex;						// �������ڵ�����������ÿһ���۵�ʱ��ֻ����turnIndex��currentTurns��ȵ�����
		int grovePos;						// ���ڲ۵�λ��
		std::function<void()> func;			// ����ִ�к���

		TaskInfo(std::function<void()> _func, std::chrono::milliseconds _interval, bool _isLoop)
		{
			func = _func;
			interval = _interval;
			isLoop = _isLoop;
		}
	};

	// ʱ����
	using TaskList = std::list<std::shared_ptr<TaskInfo>>;
	class TimeWheel
	{
	private:
		uint32_t grooveNum;					// ÿһ�ֵĲ���
		uint32_t currentTurns;				// ��ǰ������
		int timeStep;						// ��ѭ��ʱ�䲽������λ����ms

		int tick;							// ʱ���ֵ�ָ�루ÿ���ƶ�һ����λ��

		std::atomic_uint ids = 1;				// ÿ����һ����ʱ������һ��id

		std::vector<TaskList> taskWheel;

		std::shared_ptr<std::thread> loopThread;		// ʱ����ѭ�߳�

		std::map<uint32_t, int> taskManager;		// ͨ��id�ҵ�ĳ���������ڲ���<id, grovePos>��

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
			// �����̵߳���ѭ��
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
				std::this_thread::sleep_for(std::chrono::milliseconds(timeStep));	// ÿ��ѭ��˯��ָ���ƶ�һ���ʱ��
				tickForward();
				processTaskList();
			}
		}

		// ʱ����ǰ��
		void tickForward()
		{
			tick++;
			auto n1 = tick / grooveNum;
			if (n1 >= 1)
			{// ������һ��
				auto remainder = tick % grooveNum;
				tick = remainder;
				currentTurns++;
			}
		}

		// ����ǰʱ���µ������б�
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
					// �������list���Ƴ�
					it = taskList.erase(it);

					// ִ�ж�ʱ����
					pTask->func();
					// �����Ƿ�ѭ�����·�������λ��
					if (pTask->isLoop)
					{
						insertTask(pTask);
					}
				}
				else
					it++;
			}
		}

		// �������
		void insertTask(std::shared_ptr<TaskInfo>& taskInfo)
		{
			taskInfo->interval;

			auto moveStep = taskInfo->interval.count() / timeStep;		// ������ʱ����ָ����Ҫ�ߵĴ���
			auto totalTick = tick + moveStep;

			int resTurns = totalTick / grooveNum + currentTurns;
			int resTick = totalTick % grooveNum;

			taskInfo->turnIndex = resTurns;
			taskInfo->grovePos = resTick;

			taskManager[taskInfo->id] = resTick;	// ��¼�����ڵڼ�����

			taskWheel[resTick].push_back(taskInfo);
		}

		//// �������ӿ�
		//void insertTask(std::function<void>& func, std::chrono::milliseconds interval, bool isLoop = true)
		//{
		//	auto taskInfo = std::make_shared<TaskInfo>(func, interval, isLoop);
		//	// ���ݼ��������Ҫ������һ�����һ���� 	


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