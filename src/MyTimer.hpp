#pragma once
#include<chrono>
#include<thread>
#include<vector>
#include<functional>
#include<list>
#include<atomic>
#include<map>
#include<iostream>

#define GROOVE_NUM 200				// ÿ��Ĳ���
#define TIME_STEP 10				// ʱ�䲽��(ms)
#define LAYER_NUM 5					// ʱ���ֵĲ���

namespace Timer
{
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

		bool isLoop;
		std::chrono::milliseconds interval;
		int layerIndex;						// �������ڵĲ���
		std::function<void()> func;			// ����ִ�к���

		std::map<short, int> posInfo;		// ��������ʱ��ÿһ���߹��ĸ���

		bool isActive = true;

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
		std::atomic_uint ids = 1;
		std::map<uint32_t, std::weak_ptr<TaskInfo>> taskMap;	// ͨ��id��ѯ������
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

		// ʱ����ǰ��һ����λ���ӵ׵��߲�˳�����ÿһ���ʱ���֣�ǰһ�㷢����λ��һ��ָ���ƶ�һ��
		// ����ÿһ���ʱ���֣�ֻҪָ�뷢���ƶ�����Ҫ������ָ��λ���µ�����
		void timeForward();

		// ������ײ��ʱ���ֶ���Ҫ�����񽵼�����
		void processOneWheelTask(Wheel& wheel, int index);
		void removeTaskFromMap(uint32_t id);

		// ��ȡ��ǰ�ֵ���һ��
		Wheel& getLowerWheel(int curIndex);
		Wheel& getUpperWheel(int curIndex);

		// ������񣬸���interval�����
		void insertTask(std::shared_ptr<TaskInfo> taskInfo);
		void insertToTargetPos(std::shared_ptr<TaskInfo> taskInfo, int wheelIndex, int groovePos);
	};



	// ����ӿ�
	uint32_t schedule(std::function<void()> func, float seconds);
	uint32_t scheduleOnce(std::function<void()> func, float seconds);
	void unSchedule(uint32_t id);
}