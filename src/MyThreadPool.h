#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include<functional>
#include<mutex>
#include<thread>
#include<queue>
#include<future>
#include<map>
#include<set>
#include<atomic>
#include<list>

// �������߳���
#define THREAD_NUM 8

namespace threadUtil
{
	// һ�������ʵ��
	// ��һ��������Ҫ�����Լ������ݣ��������Լ��ķ�����ʱ��̳д��ಢ��дrun����
	class TaskEntry
	{
	public:
		// ÿһ�����񶼰�������˳��ִ��
		virtual void start(){}
		virtual bool run() = 0;
		virtual void end() {
			//delete this;
		}
	};

	// һ������ֻ��һ��������ʱ��
	//template<typename T> // ֧�����зº�����ʽ����
	class FuncTask : public TaskEntry
	{
	public:
		FuncTask(const std::function<void()>& func) :processFunc(func)
		{
		}

		bool run() override
		{
			processFunc();
			return true;
		}

		std::function<void()> processFunc;
	};

	enum class POOL_STATE
	{
		RUNNING,	// ��������
		STOPPING,	// ����ֹͣ������ʣ������
		STOP		// ֹͣ״̬
	};

	// ��֧����һ�����������߳�ִ�й����� ���һ���������ҵȴ���������� ֻ������������
	class ThreadPool
	{
	public:
		using Func = std::function<void()>;

		static ThreadPool& getInstance()
		{
			static ThreadPool instance;
			return instance;
		}

		ThreadPool() :state(POOL_STATE::STOP)
		{
			start(THREAD_NUM);
		}

		~ThreadPool()
		{
			if (isRunning())
			{
				forceStop();
			}
			std::cout << "thread pool destroy" << std::endl;
		}
		// �����̳߳�
		void start(int num = 4);
		// �������������һ�����̣�
		void addTask(const Func& func)
		{
			std::unique_lock<std::mutex> lck(mutex);
			std::unique_lock<std::mutex> lck2(mxWaitTask);
			if (isRunning())
			{
				taskList.push_back(std::make_shared<FuncTask>(func));
				cvTaskAdd.notify_one();
			}
		}
		// �������������һ��������TaskEntry�Ķ���
		void addTask(std::shared_ptr<TaskEntry> ptrTaskEntry)
		{
			std::unique_lock<std::mutex> lck(mutex);
			std::unique_lock<std::mutex> lck2(mxWaitTask);
			if (isRunning())
			{
				taskList.push_back(ptrTaskEntry);
				cvTaskAdd.notify_one();
			}
		}

		// �ȴ�����������ɣ�����֮���ڵ�ǰ������������֮ǰ����µ����񶼽�������ȷ�����õ�ʱ���Ѿ����������������
		int waitAllTaskFinish();
		// ����ֹͣ�̳߳أ�����ʣ��δִ�е�task
		void forceStop();
		// ���ٽ������񣬵��������������֮��ֹͣ�̳߳�
		void waitStop();

		POOL_STATE getState() { return state; }

		bool isRunning() { return state == POOL_STATE::RUNNING; }
		bool isStopping() { return state == POOL_STATE::STOPPING; }
		bool isStop() { return state == POOL_STATE::STOP; }

		bool isTaskRunning() { return !taskRunningList.empty(); }
		bool isTaskPending() { return !taskList.empty(); }
		bool isNoTask() { return (!isTaskRunning() && !isTaskPending()); }

	private:
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;

		void threadLoop();

		std::shared_ptr<TaskEntry> getTask();

		void stop(bool isWait);
		void reset();
		void setPoolState(POOL_STATE _state);

		std::mutex mutex;							// �����б�Ĳ�����
		std::mutex mxWaitTask;						// �������ʱ��Ҫ����û�н���ȴ������������
		std::mutex mxRunningList;					// �б��ѯ����

		std::condition_variable cvTaskAdd;			// ������ӵ���������
		int threadNum;								// ������߳�����
		std::vector<std::shared_ptr<std::thread>> threadList;

		//std::deque<Func> taskList;
		std::list<std::shared_ptr<TaskEntry>> taskList;

		std::set<std::shared_ptr<TaskEntry>> taskRunningList;	// �������е������б�

		bool isAcceptTask = true;	// �Ƿ���Խ�������

		POOL_STATE state = POOL_STATE::STOP;		// �̳߳ص�״̬
	};

	// ���ڸ�Ŀ�꺯���ַ��߳���Դ
	// ��������ֵ����future��future����������������
	// ִ�к����з���ֵ��
	template<typename T>
	typename std::enable_if<!std::is_void<T>::value, std::future<T>>::type myAsync(std::function<T()> processFunc)
	{
		auto ptrPromise = std::make_shared<std::promise<T>>();
		auto& myThreadPool = ThreadPool::getInstance();
		myThreadPool.addTask([ptrPromise, processFunc]()
			{
				auto value = processFunc();
				ptrPromise->set_value(value);
			}
		);

		return ptrPromise->get_future();
	}

	// ִ�к���Ϊvoid���͵�
	template<typename T>
	typename std::enable_if<std::is_void<T>::value, std::future<T>>::type myAsync(std::function<T()> processFunc)
	{
		auto ptrPromise = std::make_shared<std::promise<T>>();
		auto& myPool = ThreadPool::getInstance();
		myPool.addTask([ptrPromise, processFunc]()
			{
				processFunc();
				ptrPromise->set_value();
			}
		);

		return ptrPromise->get_future();
	}
}