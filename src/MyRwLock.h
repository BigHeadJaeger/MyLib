#pragma once
#include<shared_mutex>

// 读写锁

//shared_lock 内含一个mutex的指针，=赋值的时候是深拷贝，在构造时传入的mutex会自动lock_shared（可共享）
//析构的时候mutex会进行unlock_shared

//unique_lock与shared_lock的区别是mutex会调用lock（不可共享）

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


// 利用读写锁控制一块内存的读写
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