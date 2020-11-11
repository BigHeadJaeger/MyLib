#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include<functional>
#include<mutex>
#include<thread>
#include<queue>

class ThreadPool
{
public:
	using taskFunc = std::function<void()>;

	static ThreadPool& getInstance()
	{
		static ThreadPool instance;
		return instance;
	}


	ThreadPool(int num = 3)
	{
		threadNum = num;
		isRunning = false;
	}
	~ThreadPool()
	{
		if(isRunning)
			stop();
	}

	void start();
	void stop();

	void addTask(const taskFunc& func);

private:
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	void threadLoop();

	std::function<void()> getTask();

	std::mutex mutex;
	std::condition_variable condition;
	int threadNum;				// 管理的线程总数
	bool isRunning = false;		// 是否正在运行
	bool noMoreTask = false;	// 标志不会再有任务进入了
	std::vector<std::shared_ptr<std::thread>> threadList;

	std::deque<taskFunc> taskList;

};