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

namespace threadUtil
{
	enum class POOL_STATE
	{
		RUNNING,	// ��������
		STOPPING,	// ����ֹͣ������ʣ������
		STOP		// ֹͣ״̬
	};


	// ��֧����һ�������� ���һ���������ҵȴ���������� ֻ������������
	class ThreadPool
	{
	public:
		using taskFunc = std::function<void()>;

		static ThreadPool& getInstance()
		{
			static ThreadPool instance;
			return instance;
		}

		ThreadPool() :state(POOL_STATE::STOP)
		{
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
		// �������
		void addTask(const taskFunc& func);
		// �ȴ�����������ɣ�����֮���ڵ�ǰ������������֮ǰ����µ����񶼽��ȴ���ȷ�����õ�ʱ���Ѿ����������������
		int waitAllTaskFinish()
		{
			std::unique_lock<std::mutex> lck(mxWaitTask);
			while (!isStop()&&!taskList.empty())
			{
				cvWaitTaskFinish.wait(lck);
			}

			if (!isStop())
				return 1;
			else
				return 0;
		}

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

		std::shared_ptr<taskFunc> getTask();

		void stop(bool isWait);
		void reset();
		void setPoolState(POOL_STATE _state);

		std::mutex mutex;							// �����б�Ĳ�����
		std::mutex mxWaitTask;						// �������ʱ����
		std::condition_variable cvTaskAdd;			// ������ӵ���������
		std::condition_variable cvWaitTaskFinish;			// �ȴ�����������ɵ���������
		int threadNum;								// ������߳�����
		std::vector<std::shared_ptr<std::thread>> threadList;

		std::deque<taskFunc> taskList;

		//std::vector<>
		std::set<std::shared_ptr<taskFunc>> taskRunningList;	// �������е������б�

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