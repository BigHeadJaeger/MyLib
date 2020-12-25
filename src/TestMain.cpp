#include"MyThreadPool.h"
#include<atomic>
#include<future>
#include<tuple>
#include<queue>
#include<memory>
#include<chrono>
#include<list>
#include<map>
#include<atltime.h>
#include<assert.h>
#include<array>

#include"Common.h"

#include"MyTimer.hpp"
#include"ThreadTimer.hpp"
#include"Algorithm.hpp"

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

//int main()
//{
//	//auto a = testQ.back();
//	//a = testQ.front();
//	//testQ.pop();
//
//	//auto& myPool = ThreadPool::getInstance();
//	//myPool.start(2);
//
//	//auto res = myAsync<int>([]() {
//	//	std::cout << std::this_thread::get_id() << std::endl;
//	//	this_thread::sleep_for(chrono::seconds(2));
//	//	std::cout << std::this_thread::get_id() << " end" << std::endl;
//	//	return 1;
//	//	}
//	//).get();
//
//	//for (size_t i = 0; i < 3; i++)
//	//{
//	//	//myPool.addTask([]() {
//	//	//	std::cout << std::this_thread::get_id() << std::endl;
//	//	//	this_thread::sleep_for(chrono::seconds(2));
//	//	//	std::cout << std::this_thread::get_id() << " end" << std::endl;
//	//	//	});
//
//	//	myAsync<void>([]() {
//	//		std::cout << std::this_thread::get_id() << std::endl;
//	//		this_thread::sleep_for(chrono::seconds(2));
//	//		std::cout << std::this_thread::get_id() << " end" << std::endl;
//	//		}
//	//	);
//	//}
//
//	//ThreadPool::getInstance().waitAllTaskFinish();
//
//	////myPool.waitAllTaskFinish();
//
//	//cout << "task all finish" << endl;
//
//
//	//for (int i = 0; i < 4; i++)
//	//{
//	//	pool.addTask(testFunc);
//	//}
//
//	//auto res = newThreadFunc<int>([]() {
//	//	std::cout << std::this_thread::get_id() << std::endl;
//	//	std::this_thread::sleep_for(std::chrono::seconds(1));
//	//	std::cout << std::this_thread::get_id() << std::endl;
//	//	return 1;
//	//	}
//	//).get();
//
//
//	//vector<thread> list;
//
//	//for (size_t i = 0; i < 20; i++)
//	//{
//	//	list.push_back(thread([]()
//	//		{
//	//			std::cout << std::this_thread::get_id() << std::endl;
//	//			std::this_thread::sleep_for(std::chrono::seconds(1));
//	//			std::cout << std::this_thread::get_id() << " end" << std::endl;
//	//		}));
//
//	//}
//
//	//for (auto& element : list)
//	//{
//	//	element.join();
//	//}
//
//	//auto task = make_shared<MyTask>();
//
//	//ThreadPool::getInstance().addTask(task);
//
//	//getchar();
//
//
//	//chrono::milliseconds t1(100);
//
//	//Timer::TimeWheel myWheel;
//	//auto n1 = myWheel.addNewTask([]() {
//	//	cout << "sdsdsd" << endl;
//	//	}, t1, false);
//
//	//auto a = chrono::system_clock::now();
//	//auto tt = chrono::system_clock::to_time_t(a);
//	//char str[30];
//	//ctime_s(str, sizeof(str), &tt);
//	//printf_s("%s", str);
//
//	//Timer::schedule([]() {
//	//	auto a = chrono::system_clock::now();
//	//	auto tt = chrono::system_clock::to_time_t(a);
//	//	char str[30];
//	//	ctime_s(str, sizeof(str), &tt);
//	//	printf_s("%s", str);
//	//	cout << "xxxx" << endl;
//	//}, 2);
//
//
//	//map<int, int> map1;
//	//map1.insert({ 3,1 });
//	//map1.insert({ 4,2 });
//
//	//auto it = map1.find(3);
//	//map1.erase(it);
//
//	//int a = -1;
//	//if (a)
//	//{
//	//	cout << "ss";
//	//}
//	//else
//	//{
//	//	cout << "xx";
//	//}
//
//
//
//	//auto a = chrono::system_clock::now();
//	//auto tt = chrono::system_clock::to_time_t(a);
//	//char str[30];
//	//ctime_s(str, sizeof(str), &tt);
//	//printf_s("%s", str);
//	////uint32_t scheID = 0;
//	////scheID = Timer::schedule([&]() {
//	////	printTime(scheID);
//	////}, 2);
//
//	////Timer::scheduleOnce([&]() {
//	////	Timer::unSchedule(scheID);
//	////}, 5);
//
//	Timer::schedule([]() {
//		printTime(1);
//	}, 0.01);
//	
//	//struct TestS
//	//{
//	//	int a;
//	//	string b;
//	//	TestS() {
//	//		cout << "sss" << endl;
//	//	}
//	//	TestS(int _a, string _b)
//	//	{
//	//		a = _a;
//	//		b = _b;
//	//	}
//	//};
//
//	//TestS sarray[] = {
//	//	{2, "sdd"},
//	//	{3, "xxx"}
//	//};
//
//
//
//	getchar();
//	
//	return 0;
//}

struct S1
{
	int a;
	int b;
	string c;
	S1()
	{
		a = 3;
		b = 6;
		c = "uuuu";
	}
};

struct S2
{
	S1 s1;
	int q;
	string d;
	S2()
	{
		q = 33;
		d = "xx";
	}
};

class C3
{
private:
	static int b;
public:
	static void fun1()
	{
		cout << b << endl;
	}
};

void testfun1()
{
	static int b = 2;
	b++;
}

class BaseA
{
public:
	int s = 1;
public:
	virtual void fun1()
	{

	}

	static void fun2()
	{

	}

	~BaseA()
	{
		cout << "ssdsd";
	}
};

class DeriveB : public BaseA
{
public:
	void fun1() override
	{

	}

	~DeriveB()
	{
		cout << "vvvvv";
	}
};

class Test1
{

};

int func(int x)
{
	int countx = 0;
	while (x)
	{
		countx++;
		x = x & (x - 1);
	}
	return countx;
}

void func2(BaseA& a)
{

}

int main() 
{
	//sssss = 1;
	std::vector<int> v1{ 5,3,1,8,4,2,9 };
	//Algorithm::sort1(v1);

	Algorithm::sort5(v1);

	for (auto& item : v1)
	{
		cout << item << endl;
	}


	//std::array<int, 10> testA{ 5,5,3,12 };

	//int* a[10];
	//int(*p)[20];
	
	//CTime s(0, 0, 0, 0, 0, 0);

	//CTime time2(2020, 12, 23, 0, 0, 0);

	//CTimeSpan tsp = s - time2;

	//auto res = abs(tsp.GetDays());

	//auto s = (0 == NULL);
	//auto s1 = (0 == nullptr);
	//func(13);
	//testfun1();
	//testfun1();
	//S1* s = new S1();
	////memset(s, 0, sizeof(s));

	//S2* s2 = new S2();
	//memset(s2, 0, sizeof(*s));

	//memcpy(s2, s, sizeof(*s));

	//int a = 0;
	////a++ = 3;
	//++a = 5;

	//int c = 1;

	//const char* p = "sdsd";

	//int& const b = a;

	//b = c;

	//dynamic_pointer_cast

	//auto ptr1 = make_shared>

	//BaseA* p1 = new DeriveB();

	//DeriveB* p2 = dynamic_cast<DeriveB*>(p1);



	//shared_ptr<BaseA> ptr1 = make_shared<DeriveB>();

	//auto test = dynamic_pointer_cast<DeriveB>(ptr1);

	//auto ptr2 = make_unique<DeriveB>();

	//{
	//	DeriveB b;
	//}

	//int a = 9;

	//int c = 0;

	//const int& b = a;

	//a = 5;

	return 0;
}