// ���������ĳ��÷�����װ(һ����������һ������)
#pragma once
#include<mutex>
#include<condition_variable>
#include<list>

// Ŀ�����list�еģ�����Ŀ����˳���Ƚ��ȳ�
template<typename T>
class Condition
{
public:
	std::mutex mutex;
	std::condition_variable cv;
	std::list<T*> test;

	size_t nMaxSize;		// ��ǰ����������Ŀ������
public:
	Condition():nMaxSize(256)
	{
		
	}

	T* wait();						// ��ȡĿ��

	void notify(T* target);			// ���Ŀ��
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
