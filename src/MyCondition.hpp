// 条件变量的常用方法封装(一个条件附带一个容器)
#pragma once
#include<mutex>
#include<condition_variable>
#include<list>

// 目标放在list中的，消耗目标有顺序先进先出
template<typename T>
class Condition
{
public:
	std::mutex mutex;
	std::condition_variable cv;
	std::list<T*> test;

	size_t nMaxSize;		// 当前条件最多管理目标数量
public:
	Condition():nMaxSize(256)
	{
		
	}

	T* wait();						// 获取目标

	void notify(T* target);			// 添加目标
	void notifyAll();

};

template<typename T>
inline T* Condition<T>::wait()
{
	
	return NULL;
}

template<typename T>
inline void Condition<T>::notify(T* target)
{
	
}

template<typename T>
inline void Condition<T>::notifyAll()
{

}
