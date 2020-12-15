#include"MyThreadPool.h"
#include<atomic>
#include<future>
#include<tuple>
#include<queue>
#include<memory>
#include<chrono>
#include<list>
#include<map>
#include"Common.h"

#include"MyTimer.hpp"
#include"ThreadTimer.hpp"

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

class Base
{
private:
	int a;

protected:
	int b;
};

class IClass : public Base
{
public:
	int c;
};

class MyTask : public TaskEntry
{
private:
	bool run() override
	{
		cout << "sss" << endl;
		return true;
	}
};

void fun6(int a, int b, int c)
{
	cout << a << endl;
	cout << b << endl;
	cout << c << endl;
}

int fun7(int a)
{

	return a + 1;
}


void printTime(int id)
{
	auto a = chrono::system_clock::now();
	auto tt = chrono::system_clock::to_time_t(a);
	char str[30];
	ctime_s(str, sizeof(str), &tt);
	printf_s("%d   %s", id, str);
}

int main()
{
	//auto f1 = bind(fun6, 3, 5, placeholders::_1);

	//f1(3);


	//cout << "sss" << endl;
	//{
	//	unique_ptr<Base> base1 = make_unique<Base>();

	//	unique_ptr<Base> base2 = make_unique<IClass>();


	//	auto test = static_cast<IClass*>(base2.get());

	//	
	//	//auto test2 = dynamic_cast<IClass*>(base2.get());

	//	cout << "" << endl;
	//}


	//cout << "" << endl;
	//unique_ptr<string> b = a.get();


	//queue<int> testQ;
	//testQ.push(1);
	//testQ.push(4);
	//testQ.push(2);
	//testQ.push(3);

	//auto a = testQ.back();
	//a = testQ.front();
	//testQ.pop();

	//auto& myPool = ThreadPool::getInstance();
	//myPool.start(2);

	//auto res = myAsync<int>([]() {
	//	std::cout << std::this_thread::get_id() << std::endl;
	//	this_thread::sleep_for(chrono::seconds(2));
	//	std::cout << std::this_thread::get_id() << " end" << std::endl;
	//	return 1;
	//	}
	//).get();

	//for (size_t i = 0; i < 3; i++)
	//{
	//	//myPool.addTask([]() {
	//	//	std::cout << std::this_thread::get_id() << std::endl;
	//	//	this_thread::sleep_for(chrono::seconds(2));
	//	//	std::cout << std::this_thread::get_id() << " end" << std::endl;
	//	//	});

	//	myAsync<void>([]() {
	//		std::cout << std::this_thread::get_id() << std::endl;
	//		this_thread::sleep_for(chrono::seconds(2));
	//		std::cout << std::this_thread::get_id() << " end" << std::endl;
	//		}
	//	);
	//}

	//ThreadPool::getInstance().waitAllTaskFinish();

	////myPool.waitAllTaskFinish();

	//cout << "task all finish" << endl;


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


	//vector<thread> list;

	//for (size_t i = 0; i < 20; i++)
	//{
	//	list.push_back(thread([]()
	//		{
	//			std::cout << std::this_thread::get_id() << std::endl;
	//			std::this_thread::sleep_for(std::chrono::seconds(1));
	//			std::cout << std::this_thread::get_id() << " end" << std::endl;
	//		}));

	//}

	//for (auto& element : list)
	//{
	//	element.join();
	//}

	//auto task = make_shared<MyTask>();

	//ThreadPool::getInstance().addTask(task);

	//getchar();

	//chrono::milliseconds mscond(1000);

	//chrono::time_point<chrono::system_clock> t1 = std::chrono::system_clock::now();
	//auto s3 = chrono::duration_cast<chrono::milliseconds>(t1.time_since_epoch());
	//auto s2 = chrono::duration_cast<chrono::seconds>(t1.time_since_epoch());
	//auto s4 = chrono::duration_cast<chrono::microseconds>(t1.time_since_epoch());
	//auto s1 = t1.time_since_epoch().count();

	//chrono::milliseconds a(10);

	//chrono::duration<int, milli> test(1000);

	//cout << test.count() << endl;

	//auto p1 = make_unique<string>();
	//
	//auto& p2 = p1;



	//list<string> list1;
	//list1.push_back("xxx");
	//list1.push_back("qqq");
	//list1.push_back("wew");
	//list1.push_back("eee");
	//list1.push_back("zzz");

	//auto it = list1.begin();
	//for (it; it != list1.end(); it++)
	//{
	//	if ((*it) == "xxx")
	//	{
	//		it = list1.erase(it);
	//	}
	//}

	//chrono::milliseconds t1(100);

	//Timer::TimeWheel myWheel;
	//auto n1 = myWheel.addNewTask([]() {
	//	cout << "sdsdsd" << endl;
	//	}, t1, false);

	//auto a = chrono::system_clock::now();
	//auto tt = chrono::system_clock::to_time_t(a);
	//char str[30];
	//ctime_s(str, sizeof(str), &tt);
	//printf_s("%s", str);

	//Timer::schedule([]() {
	//	auto a = chrono::system_clock::now();
	//	auto tt = chrono::system_clock::to_time_t(a);
	//	char str[30];
	//	ctime_s(str, sizeof(str), &tt);
	//	printf_s("%s", str);
	//	cout << "xxxx" << endl;
	//}, 2);


	//map<int, int> map1;
	//map1.insert({ 3,1 });
	//map1.insert({ 4,2 });

	//auto it = map1.find(3);
	//map1.erase(it);

	//int a = -1;
	//if (a)
	//{
	//	cout << "ss";
	//}
	//else
	//{
	//	cout << "xx";
	//}



	//auto a = chrono::system_clock::now();
	//auto tt = chrono::system_clock::to_time_t(a);
	//char str[30];
	//ctime_s(str, sizeof(str), &tt);
	//printf_s("%s", str);
	////uint32_t scheID = 0;
	////scheID = Timer::schedule([&]() {
	////	printTime(scheID);
	////}, 2);

	////Timer::scheduleOnce([&]() {
	////	Timer::unSchedule(scheID);
	////}, 5);

	Timer::schedule([]() {
		printTime(1);
	}, 0.01);

	getchar();
	
	return 0;
}