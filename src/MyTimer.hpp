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
//#define TIME_STEP 10				// ʱ�䲽��

#define GROOVE_NUM 5				// ÿ��Ĳ���
#define TIME_STEP 10				// ʱ�䲽��
#define LAYER_NUM 5					// ʱ���ֵĲ���

namespace Timer
{
	//struct TaskInfo
	//{
	//	uint32_t id;
	//	bool isLoop;
	//	std::chrono::milliseconds interval;
	//	int turnIndex;						// �������ڵ�����������ÿһ���۵�ʱ��ֻ����turnIndex��currentTurns��ȵ�����
	//	int grovePos;						// ���ڲ۵�λ��
	//	std::function<void()> func;			// ����ִ�к���

	//	TaskInfo(std::function<void()> _func, std::chrono::milliseconds _interval, bool _isLoop)
	//	{
	//		func = _func;
	//		interval = _interval;
	//		isLoop = _isLoop;
	//	}
	//};

	//// ʱ����
	//using TaskList = std::list<std::shared_ptr<TaskInfo>>;
	//class TimeWheel
	//{
	//private:
	//	uint32_t grooveNum;					// ÿһ�ֵĲ���
	//	uint32_t currentTurns;				// ��ǰ������
	//	int timeStep;						// ��ѭ��ʱ�䲽������λ����ms

	//	int tick;							// ʱ���ֵ�ָ�루ÿ���ƶ�һ����λ��

	//	std::atomic_uint ids = 1;				// ÿ����һ����ʱ������һ��id

	//	std::vector<TaskList> taskWheel;

	//	std::shared_ptr<std::thread> loopThread;		// ʱ����ѭ�߳�

	//	std::map<uint32_t, int> taskManager;		// ͨ��id�ҵ�ĳ���������ڲ���<id, grovePos>��

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
	//		// �����̵߳���ѭ��
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
	//			std::this_thread::sleep_for(std::chrono::milliseconds(timeStep));	// ÿ��ѭ��˯��ָ���ƶ�һ���ʱ��
	//			tickForward();
	//			processTaskList();
	//		}
	//	}

	//	// ʱ����ǰ��
	//	void tickForward()
	//	{
	//		tick++;
	//		auto n1 = tick / grooveNum;
	//		if (n1 >= 1)
	//		{// ������һ��
	//			auto remainder = tick % grooveNum;
	//			tick = remainder;
	//			currentTurns++;
	//		}
	//	}

	//	// ����ǰʱ���µ������б�
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
	//				// �������list���Ƴ�
	//				it = taskList.erase(it);
	//				// ��id��¼map���Ƴ�

	//				// ִ�ж�ʱ����
	//				pTask->func();
	//				// �����Ƿ�ѭ�����·�������λ��
	//				if (pTask->isLoop)
	//				{
	//					insertTask(pTask);
	//				}
	//			}
	//			else
	//				it++;
	//		}
	//	}

	//	// �������
	//	void insertTask(std::shared_ptr<TaskInfo>& taskInfo)
	//	{
	//		taskInfo->interval;

	//		auto moveStep = taskInfo->interval.count() / timeStep;		// ������ʱ����ָ����Ҫ�ߵĴ���
	//		auto totalTick = tick + moveStep;

	//		int resTurns = totalTick / grooveNum + currentTurns;
	//		int resTick = totalTick % grooveNum;

	//		taskInfo->turnIndex = resTurns;
	//		taskInfo->grovePos = resTick;

	//		taskManager[taskInfo->id] = resTick;	// ��¼�����ڵڼ�����

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

	//	// ����idɾ��ĳ������
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

	// ��������Ҫת����ÿ���Ӧ��ָ���߶���
	// <0, 3> ��һ����3��
	// <1, 1> �ڶ�����1��
	// <2, 0> ��������0��
	// <3, 0> ���Ĳ���0��
	// <4, 0> �������0��
	// ��ÿһ���ָ���ߵ��˵�ǰ���������
	struct TaskInfo
	{
		uint32_t id;
		uint32_t startTime;					// �������ʱ��ʱ��

		bool isLoop;
		std::chrono::milliseconds interval;
		int layerIndex;						// �������ڵĲ���
		int groovePos;						// ���ڲ۵�λ��
		std::function<void()> func;			// ����ִ�к���

		std::map<short, int> posInfo;		// ��������ʱ��ÿһ���߹��ĸ���

		TaskInfo(std::function<void()> _func, std::chrono::milliseconds _interval, bool _isLoop)
		{
			func = _func;
			interval = _interval;
			isLoop = _isLoop;
		}
	};

	// ʱ����
	using TaskList = std::list<std::shared_ptr<TaskInfo>>;

	// һ��ʱ���ֵ���Ϣ
	class Wheel
	{
	public:
		uint32_t grooveNum;					// ÿһ�ֵĲ���
		int timeStep;						// ��ѭ��ʱ�䲽������λ����ms
		int tick;							// ʱ���ֵ�ָ�루ÿ���ƶ�һ����λ��

		std::vector<TaskList> taskWheel;	// ʱ���ֵ��������

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

		// ָ����ǰ�ƶ�
		bool tickForward()
		{
			bool isAdding = false;		// �Ƿ��λ
			tick++;
			if (tick >= grooveNum)
			{
				isAdding = true;
				tick = 0;
			}
			return isAdding;
		}
	};

	// �ֲ�ʱ����
	// ÿ��200���ۣ���һ���ָ������10ms��֮��ÿһ���ָ��������һ����ܿ��
	// 5��ɱ�ʾ��ʱ���Լ102��
	// ÿһ��ʱ���ֵĿ̶�1����ʼ���̶�0����ĩβ��10ms�������ڵ�һ�ֵ�1�̶ȣ� 200ms�������ڵ�һ�ֵ�0�̶ȣ� 210ms�ڵڶ��ֵ�1�̶ȣ�
	class LayerTimeWheel
	{
	private:
		std::vector<Wheel> wheels;		// ÿһ���ʱ���ּ���
		bool isRunning = false;
		std::shared_ptr<std::thread> loopThread;		// ʱ����ѭ�߳�
	public:
		LayerTimeWheel()
		{
			wheels.reserve(LAYER_NUM);
			int lastTotalTimes = TIME_STEP;
			// ����LAYER_NUM��ʱ���ֵ�ָ����
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
			// �����̵߳���ѭ��
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
				std::this_thread::sleep_for(std::chrono::milliseconds(TIME_STEP));	// ÿ��ѭ��˯��ָ���ƶ�һ���ʱ��
				timeForward();
				//processTaskList();
			}
		}

		static LayerTimeWheel& getInstance()
		{
			static LayerTimeWheel instance;
			return instance;
		}

		// ʱ����ǰ��һ����λ���ӵ׵��߲�˳�����ÿһ���ʱ���֣�ǰһ�㷢����λ��һ��ָ���ƶ�һ��
		// ����ÿһ���ʱ���֣�ֻҪָ�뷢���ƶ�����Ҫ������ָ��λ���µ�����
		void timeForward()
		{
			bool isAdd = true;
			for (size_t i = 0; i < wheels.size(); i++)
			{
				if (isAdd)
				{
					isAdd = wheels[i].tickForward();
					if (isAdd && i == (wheels.size() - 1))
					{// һ�㲻�ᳬ������ʱ���ֿ���������102��
						std::cout << "Exceeded the maximum time limit" << std::endl;
						return;
					}
					processOneWheelTask(wheels[i], i);
				}
			}
		}

		// ������ײ��ʱ���ֶ���Ҫ�����񽵼�����
		void processOneWheelTask(Wheel& wheel, int index)
		{
			int tick = wheel.tick;
			auto& list = wheel.taskWheel[tick];
			auto it = list.begin();
			for (it; it != list.end(); it)
			{
				auto taskInfo = (*it);
				// �ӵ�ǰ�б�ɾ������
				it = list.erase(it);

				if (index == 0)
				{// �ײ�ʱ����ֱ��ִ��
					taskInfo->func();
					// �����Ƿ�ѭ�����²�������
					if (taskInfo->isLoop)
						insertTask(std::move(taskInfo));
				}
				else
				{
					// �ȿ�λ����Ϣ��ǰ��֮���Ƿ����ƶ���
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
					{// ����²�û���ƶ�����ִ������
						taskInfo->func();
						if (taskInfo->isLoop)
							insertTask(std::move(taskInfo));
					}
					else
					{
						// ˢ�������λ����Ϣ
						taskInfo->posInfo[index] = 0;
						insertToTargetPos(taskInfo, index - 1, taskInfo->posInfo[index - 1]);
					}
				}

			}

			//wheel.taskWheel[tick]
		}

		// ��ȡ��ǰ�ֵ���һ��
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

		// ������񣬸���interval�����
		void insertTask(std::shared_ptr<TaskInfo> taskInfo)
		{
			int moveStep = taskInfo->interval.count() / wheels[0].timeStep;
			int add = moveStep;
			int remain = 0;
			int resLayer = 0;

			// �ӵײ�ѭ������ÿһ��ָ����ƶ�����
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