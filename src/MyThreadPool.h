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

// 开启的线程数
#define THREAD_NUM 8

namespace threadUtil
{
	// 一个任务的实例
	// 当一个任务需要保存自己的数据，或者有自己的方法的时候继承此类并重写run函数
	class TaskEntry
	{
	public:
		// 每一个任务都按照下列顺序执行
		virtual void start(){}
		virtual bool run() = 0;
		virtual void end() {
			//delete this;
		}
	};

	// 一个任务只是一个函数的时候
	//template<typename T> // 支持所有仿函数形式的量
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
		RUNNING,	// 正在运行
		STOPPING,	// 正在停止（处理剩余任务）
		STOP		// 停止状态
	};

	// 不支持在一个任务所在线程执行过程中 添加一个新任务且等待新任务完成 只添加新任务可以
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
		// 开启线程池
		void start(int num = 4);
		// 添加任务（任务是一个方程）
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
		// 添加任务（任务是一个派生于TaskEntry的对象）
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

		// 等待所有任务完成，调用之后在当前所有任务处理完之前添加新的任务都将阻塞（确保调用的时候已经添加完了所有任务）
		int waitAllTaskFinish();
		// 立刻停止线程池，抛弃剩余未执行的task
		void forceStop();
		// 不再接受任务，等现有任务处理完毕之后停止线程池
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

		std::mutex mutex;							// 任务列表的操作锁
		std::mutex mxWaitTask;						// 添加任务时需要看有没有进入等待任务结束的锁
		std::mutex mxRunningList;					// 列表查询的锁

		std::condition_variable cvTaskAdd;			// 任务添加的条件变量
		int threadNum;								// 管理的线程总数
		std::vector<std::shared_ptr<std::thread>> threadList;

		//std::deque<Func> taskList;
		std::list<std::shared_ptr<TaskEntry>> taskList;

		std::set<std::shared_ptr<TaskEntry>> taskRunningList;	// 正在运行的任务列表

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