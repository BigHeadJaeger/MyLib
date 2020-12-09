#include"MyThreadPool.h"
#include<atomic>
#include<future>
#include<tuple>
#include<queue>
#include<memory>
#include<chrono>
#include<list>
#include"Common.h"

#include"MyTimer.hpp"

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


int main()
{

	//string* testStr = new string("sss");
	//{
	//	auto t2 = make_shared<string>(testStr);
	//}

	//cout << "Sd" << endl;




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

	//string str1 = "asdf";

	//tools::processArgs([](auto&& param) {
	//	param = "qqq";
	//	}, str1);




	//auto p1 = make_shared<string>("xxxx");



	//unique_ptr<string> a = make_unique<string>(str1);

	//*a = "sdds";


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

	chrono::milliseconds t1(100);

	Timer::TimeWheel myWheel;
	auto n1 = myWheel.addNewTask([]() {
		cout << "sdsdsd" << endl;
		}, t1, false);


	getchar();
	
	return 0;
}