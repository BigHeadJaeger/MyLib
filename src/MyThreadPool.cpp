#include "MyThreadPool.h"
#include<assert.h>

using namespace threadUtil;

void ThreadPool::start(int num)
{
	if (isRunning())
	{
		//std::cout << "thread pool already started" << std::endl;
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

//void ThreadPool::addTask(const Func& func)
//{
//	std::unique_lock<std::mutex> lck(mutex);
//	std::unique_lock<std::mutex> lck2(mxWaitTask);
//	//if(noMoreTask)
//	if (isRunning())
//	{
//		taskList.push_back(func);
//		cvTaskAdd.notify_one();
//	}
//}

int threadUtil::ThreadPool::waitAllTaskFinish()
{
	std::unique_lock<std::mutex> lck(mxWaitTask);
	while (!isStop() && (!taskList.empty() || !taskRunningList.empty()))
	{
		std::this_thread::yield();
	}

	// 等待任务全部完成的过程中如果出发了stop，返回0
	if (!isStop())
		return 1;
	else
		return 0;
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
			task->start();

			task->run();
			//(*task.get())();

			mxRunningList.lock();
			auto it = taskRunningList.find(task);
			if (it != taskRunningList.end())
			{
				taskRunningList.erase(it);
			}
			else
			{
				std::cout << "error" << std::endl;
			}
			mxRunningList.unlock();

			task->end();
		}
	}
}

std::shared_ptr<TaskEntry> ThreadPool::getTask()
{
	std::unique_lock<std::mutex> lck(mutex);

	while (taskList.empty() && isRunning())
	{
		cvTaskAdd.wait(lck);
	}

	std::shared_ptr<TaskEntry> task;

	if (!taskList.empty())	// 如果线程池已经停止了则任务列表一定是清空的
	{
		task = taskList.front();
		taskList.pop_front();
		mxRunningList.lock();
		taskRunningList.insert(task);
		mxRunningList.unlock();
	}

	return task;
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
		cvTaskAdd.notify_all();		// 解除所有条件的wait
	}
	else
	{
		setPoolState(POOL_STATE::STOP);
		taskList.clear();
		cvTaskAdd.notify_all();		// 解除所有条件的wait
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
	taskRunningList.clear();
}

void threadUtil::ThreadPool::setPoolState(POOL_STATE _state)
{
	if (state == _state)
		return;
	// 设置状态的时候加锁
	std::unique_lock<std::mutex> lck(mutex);
	state = _state;
	switch (_state)
	{
	case threadUtil::POOL_STATE::RUNNING:
		std::cout << "thread pool running" << std::endl;
		break;
	case threadUtil::POOL_STATE::STOPPING:
		std::cout << "thread pool is stopping, can not add task anymore" << std::endl;
		break;
	case threadUtil::POOL_STATE::STOP:
		std::cout << "thread pool stop" << std::endl;
		break;
	default:
		break;
	}
}
