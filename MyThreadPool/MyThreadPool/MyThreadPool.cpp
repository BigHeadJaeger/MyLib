#include "MyThreadPool.h"
#include<assert.h>

void ThreadPool::start()
{
	assert(threadList.empty());
	isRunning = true;
	for (int i = 0; i < threadNum; i++)
	{
		threadList.push_back(std::make_shared<std::thread>(std::bind(&ThreadPool::threadLoop, this)));
	}
}

void ThreadPool::stop()
{
	isRunning = false;
	condition.notify_all();

	for (auto& th : threadList)
	{
		th->join();
	}
}

void ThreadPool::addTask(const taskFunc& func)
{
	std::unique_lock<std::mutex> lck(mutex);
	taskList.push_back(func);
	condition.notify_one();
}

void ThreadPool::threadLoop()
{
	while (isRunning)
	{
		if (noMoreTask && taskList.empty())
			return;
		auto task = getTask();
		if (task)
		{
			task();
		}
	}
}

std::function<void()> ThreadPool::getTask()
{
	std::unique_lock<std::mutex> lck(mutex);

	while (taskList.empty() && isRunning)
	{
		condition.wait(lck);
	}

	taskFunc func;
	if(isRunning)
	{
		func = taskList.front();
		taskList.pop_front();
	}

	return func;
}
