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
		RUNNING,	// 正在运行
		STOPPING,	// 正在停止（处理剩余任务）
		STOP		// 停止状态
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
		// 立刻停止线程池，抛弃剩余未执行的task
		void forceStop();

		// 不再接受任务，等现有任务处理完毕之后停止线程池
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
			// 设置状态的时候加锁
			std::unique_lock<std::mutex> lck(mutex);
			state = _state;
		}

		std::mutex mutex;
		std::condition_variable condition;
		int threadNum;				// 管理的线程总数
		std::vector<std::shared_ptr<std::thread>> threadList;

		std::deque<taskFunc> taskList;

		bool isAcceptTask = true;	// 是否可以接受任务

		POOL_STATE state = POOL_STATE::STOP;		// 线程池的状态
	};

	// 用于给目标函数分发线程资源
	// 函数返回值都是future，future的类型有以下两种
	// 执行函数有返回值的
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

	// 执行函数为void类型的
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