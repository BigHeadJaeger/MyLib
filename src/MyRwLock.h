#pragma once
#include<shared_mutex>

// ��д��

//shared_lock �ں�һ��mutex��ָ�룬=��ֵ��ʱ����������ڹ���ʱ�����mutex���Զ�lock_shared���ɹ���
//������ʱ��mutex�����unlock_shared

//unique_lock��shared_lock��������mutex�����lock�����ɹ���

class RWLock
{
public:
	std::shared_lock<std::shared_mutex> readLock()
	{
		return std::shared_lock<std::shared_mutex>(mutex);
	}

	std::unique_lock<std::shared_mutex> writeLock()
	{
		return std::unique_lock<std::shared_mutex>(mutex);
	}

public:
	std::shared_mutex mutex;
};


// ���ö�д������һ���ڴ�Ķ�д
template<typename T>
class RWData
{
public:
	//RWData():

	T getData()
	{
		auto guard = rwLock.readLock();
		return m_data;
	}

	std::shared_lock<std::shared_mutex> read(const T*& target)
	{
		auto guard = rwLock.readLock();
		target = &m_data;
		return guard;
	}

	std::unique_lock<std::shared_mutex> write(T*& target)
	{
		auto guard = rwLock.writeLock();
		target = &m_data;
		return guard;
	}

private:
	RWLock rwLock;
	T m_data;
};

#define RWDATA_READ(rwData, target)	\
	const decltype(rwData.getData())* target = nullptr;	\
	auto readGuard = rwData.read(target);
	
#define RWDATA_WRITE(rwData, target) \
	decltype(rwData.getData())* target = nullptr; \
	auto writeGuard = rwData.write(target);