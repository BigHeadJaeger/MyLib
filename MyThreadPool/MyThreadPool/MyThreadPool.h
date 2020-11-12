#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include<functional>
#include<mutex>
#include<thread>
#include<queue>
#include<future>

namespace threadUtil
{
	enum class POOL_STATE
	{
		RUNNING,	// ��������
		STOPPING,	// ����ֹͣ������ʣ������
		STOP		// ֹͣ״̬
	};

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

		void start(int num = 4);
		// ����ֹͣ�̳߳أ�����ʣ��δִ�е�task
		void forceStop();

		// ���ٽ������񣬵��������������֮��ֹͣ�̳߳�
		void waitStop();

		void addTask(const taskFunc& func);

		POOL_STATE getState() { return state; }

		//void 



		bool isRunning() { return state == POOL_STATE::RUNNING; }
		bool isStopping() { return state == POOL_STATE::STOPPING; }
		bool isStop() { return state == POOL_STATE::STOP; }

	private:
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;

		void threadLoop();

		std::function<void()> getTask();

		void stop(bool isWait);
		void reset();
		void setPoolState(POOL_STATE _state)
		{
			// ����״̬��ʱ�����
			std::unique_lock<std::mutex> lck(mutex);
			state = _state;
		}

		std::mutex mutex;
		std::condition_variable condition;
		int threadNum;				// ������߳�����
		std::vector<std::shared_ptr<std::thread>> threadList;

		std::deque<taskFunc> taskList;

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

//class TaskDistributor
//{
//public:
//
//};