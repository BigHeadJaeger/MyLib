#include "MyThreadPool.h"
#include<assert.h>

using namespace threadUtil;

void ThreadPool::start(int num)
{
	if (isRunning())
	{
		std::cout << "thread pool already started" << std::endl;
		return;
	}
	threadNum = num;
	assert(threadList.empty());
	//isRunning = true;
	setPoolState(POOL_STATE::RUNNING);
	for (int i = 0; i < threadNum; i++)
	{
		threadList.push_back(std::make_shared<std::thread>(std::bind(&ThreadPool::threadLoop, this)));
	}

}

void ThreadPool::forceStop()
{
	stop(false);
}

void threadUtil::ThreadPool::waitStop()
{
	stop(true);
}

void ThreadPool::addTask(const taskFunc& func)
{
	std::unique_lock<std::mutex> lck(mutex);
	//if(noMoreTask)
	if (isRunning())
	{
		taskList.push_back(func);
		condition.notify_one();
	}
}

void ThreadPool::threadLoop()
{
	while (isRunning() || isStopping())
	{
		if (isStopping() && taskList.empty())
		{
			// 跳出循环，当前线程结束
			break;
		}
			
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

	// 是否存在线程池正在停止 当前线程正在等待 且taskList不为空的情况
	while (taskList.empty() && isRunning())
	{
		condition.wait(lck);
	}

	taskFunc func = nullptr;
	if (!taskList.empty())	// 如果线程池已经停止了则任务列表一定是清空的
	{
		func = taskList.front();
		taskList.pop_front();
	}

	return func;
}

void ThreadPool::stop(bool isWait)
{
	if (!isRunning())
	{
		std::cout << "thread pool not running" << std::endl;
		return;
	}

	if (isWait)
	{
		setPoolState(POOL_STATE::STOPPING);
		condition.notify_all();		// 解除所有条件的wait
	}
	else
	{
		setPoolState(POOL_STATE::STOP);
		taskList.clear();
		condition.notify_all();		// 解除所有条件的wait
	}

	for (auto& th : threadList)
	{
		th->join();
	}

	reset();
}

void ThreadPool::reset()
{
	setPoolState(POOL_STATE::STOP);
	taskList.clear();
	threadList.clear();
}
