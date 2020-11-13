#include"MyThreadPool.h"
#include<atomic>
#include<future>
#include<tuple>
#include<queue>
#include"Common.h"

using namespace threadUtil;

using namespace std;

//std::mutex testMutex;
//std::atomic_int nA = 0;

void testFunc(int a)
{
	//std::unique_lock<std::mutex> lock(testMutex);
	std::cout << std::this_thread::get_id() << std::endl;
	//std::cout << "threadid: " << std::this_thread::get_id() << " start" << std::endl;
	//int a = 0;
	//for (size_t i = 0; i < 10000000; i++)
	//{
	//	nA++;
	//}

	a = 1;

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

int sum = 0;

template<typename T>
T fun2(T num)
{
	return num;
}


template<typename A, typename ...T>
A fun2(A first, T... args)
{
	//sum += first;
	return first + fun2<A>(args...);
	//cout << sizeof...(args) << endl;
}

template<typename T>
void fun3(T a)
{
	cout << a << endl;
}

template<typename... T>
void expand(T... args)
{
	int temp[] = { (fun3(args), 0)... };
}

template<typename F, typename... Args>
void expand2(F processFunc, Args... args)
{
	initializer_list<int>({ (processFunc(args), 0)... });
}

template<typename T>
void excuteFun(T param)
{
	cout << param << endl;
}

template<typename... Args>
void testFun1(Args&& ... args)
{
	tools::processArgs([](auto param) {
		cout << param << endl;
		}, std::forward<Args>(args)...);
}

recursive_mutex rm;

void testFun2()
{
	lock_guard<recursive_mutex> lck(rm);
	cout << "start" << endl;
	this_thread::sleep_for(chrono::seconds(2));
	cout << "end" << endl;
}


int main()
{

	

	//int a = 1;
	//int b = 3;

	//auto test = (a += b, a);

	//int total = fun2(3, 4, 5);
	//expand(4, 23, 2, "sd");

	//expand2([](auto a) {
	//	cout << a << endl;
	//	}, 1, 3, "Sds");

	//expand2(excuteFun<int>, 1, 3);

	//testFun1(3, 4, 5);

	//tools::processArgs([](auto param) {
	//	cout << param << endl;
	//	}, 2, 3, "sd");



	//queue<int> testQ;
	//testQ.push(1);
	//testQ.push(4);
	//testQ.push(2);
	//testQ.push(3);

	//auto a = testQ.back();
	//a = testQ.front();
	//testQ.pop();

	auto& myPool = ThreadPool::getInstance();
	myPool.start(2);

	//auto res = myAsync<int>([]() {
	//	std::cout << std::this_thread::get_id() << std::endl;
	//	this_thread::sleep_for(chrono::seconds(2));
	//	std::cout << std::this_thread::get_id() << " end" << std::endl;
	//	return 1;
	//	}
	//).get();


	for (size_t i = 0; i < 3; i++)
	{
		//myPool.addTask([]() {
		//	std::cout << std::this_thread::get_id() << std::endl;
		//	this_thread::sleep_for(chrono::seconds(2));
		//	std::cout << std::this_thread::get_id() << " end" << std::endl;
		//	});

		myAsync<void>([]() {
			std::cout << std::this_thread::get_id() << std::endl;
			this_thread::sleep_for(chrono::seconds(2));
			std::cout << std::this_thread::get_id() << " end" << std::endl;
			}
		).get();
	}

	myPool.waitAllTaskFinish();

	cout << "task all finish" << endl;


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

	for (size_t i = 0; i < 5; i++)
	{
		//auto res = myAsync<int>([]() {
		//	std::cout << std::this_thread::get_id() << std::endl;
		//	std::this_thread::sleep_for(std::chrono::seconds(1));
		//	std::cout << std::this_thread::get_id() << std::endl;
		//	return 1;
		//	}).get();
		//myAsync<int>([]() {
		//	std::cout << std::this_thread::get_id() << std::endl;
		//	std::this_thread::sleep_for(std::chrono::seconds(2));
		//	std::cout << std::this_thread::get_id() << std::endl;
		//	return 1;
		//	});

		//myPool.addTask(std::bind(testFunc, 2));
	}

	//std::this_thread::sleep_for(std::chrono::seconds(1));
	//myPool.forceStop();


	//myPool.
	//myPool.waitStop();


	//auto res = myAsync<std::tuple<int, std::string, int>>([]() 
	//	{
	//		auto test = std::make_tuple(2, "ere", 4);
	//		return test;
	//	}
	//).get();


	getchar();

	return 0;
}