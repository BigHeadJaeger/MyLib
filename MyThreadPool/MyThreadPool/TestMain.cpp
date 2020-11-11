#include"MyThreadPool.h"
#include<atomic>
#include<future>

using namespace threadUtil;

//std::mutex testMutex;
//std::atomic_int nA = 0;

void testFunc()
{
	//std::unique_lock<std::mutex> lock(testMutex);
	std::cout << std::this_thread::get_id() << std::endl;
	//std::cout << "threadid: " << std::this_thread::get_id() << " start" << std::endl;
	//int a = 0;
	//for (size_t i = 0; i < 10000000; i++)
	//{
	//	nA++;
	//}
	std::this_thread::sleep_for(std::chrono::seconds(1));
	//std::cout << "threadid: " << std::this_thread::get_id() << " excuted" << std::endl;
	std::cout << std::this_thread::get_id() << std::endl;
}

//template<typename R>
//typename std::enable_if<!std::is_void<R>::value, std::future<R>>::type newThreadFunc(std::function<R()> processFunc)
//{
//	auto ptrPromise = std::make_shared<std::promise<R>>();
//
//	auto& myPool = ThreadPool::getInstance();
//	myPool.addTask([ptrPromise, processFunc]() {
//		auto value = processFunc();
//		ptrPromise->set_value(value);
//		}
//	);
//
//	return ptrPromise->get_future();
//}

int main()
{
	auto& myPool = ThreadPool::getInstance();
	myPool.start();
	//for (int i = 0; i < 4; i++)
	//{
	//	pool.addTask(testFunc);
	//}

	//auto res = newThreadFunc<int>([]() {
	//	std::cout << std::this_thread::get_id() << std::endl;
	//	std::this_thread::sleep_for(std::chrono::seconds(1));
	//	std::cout << std::this_thread::get_id() << std::endl;
	//	return 1;
	//	}
	//).get();

	//std::cout << "res = " << res << std::endl;

	//auto res = myAsync<int>([]()
	//	{
	//		std::cout << std::this_thread::get_id() << std::endl;
	//		std::this_thread::sleep_for(std::chrono::seconds(1));
	//		std::cout << std::this_thread::get_id() << std::endl;
	//		return 1;
	//	}
	//).get();

	//if (res == 1)
	//{
	//	std::cout << "ssss" << std::endl;
	//}

	//myAsync<void>([]()
	//	{
	//		std::cout << std::this_thread::get_id() << std::endl;
	//		std::this_thread::sleep_for(std::chrono::seconds(1));
	//		std::cout << std::this_thread::get_id() << std::endl;
	//	}
	//).get();

	//auto res = std::async(std::launch::async, []() {

	//	return 1;
	//	}).get();

	auto res = myAsync<int>([]() {
		std::cout << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << std::this_thread::get_id() << std::endl;
		return 1;
	}).get();





	getchar();

	return 0;
}