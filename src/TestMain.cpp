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
#include<stack>
#include<math.h>
#include<random>
#include<time.h>
#include<ctime>
#include<direct.h>
#include<Windows.h>

#include<shared_mutex>

#include"Common.h"

#include"MyTimer.hpp"
#include"ThreadTimer.hpp"
#include"Algorithm.hpp"

#include <DbgHelp.h>

#define _CRT_SECURE_NO_WARNINGS

#include<string.h>

static void WriteMiniDMP(struct _EXCEPTION_POINTERS* pExp)
{
	CString   strDumpFile;
	char* szFilePath;
	szFilePath = _getcwd(NULL, 0);

	if (szFilePath)
	{
		*strrchr(szFilePath, '\\') = 0;
		auto test = CTime::GetCurrentTime().GetTickCount();
		strDumpFile.Format(L"%s\\%d.dmp", szFilePath, 100);
		HANDLE   hFile = CreateFile(strDumpFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			MINIDUMP_EXCEPTION_INFORMATION   ExInfo;
			ExInfo.ThreadId = ::GetCurrentThreadId();
			ExInfo.ExceptionPointers = pExp;
			ExInfo.ClientPointers = NULL;
			//   write   the   dump
			BOOL   bOK = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithFullMemoryInfo, &ExInfo, NULL, NULL);
			CloseHandle(hFile);
		}
	}
}

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


template<typename T>
struct Node
{
	T data;
	Node* next;
};

template<typename T>
void flip(Node<T>* head)
{
	Node<T>* current = head;
	Node<T>* last = nullptr;
	while (current->next != nullptr)
	{
		Node<T>* next = current->next;
		current->next = last;
		last = current;
		current = next;
	}
	current->next = last;
	last = nullptr;
}
template<typename T>
void printNum(Node<T>* head)
{
	Node<T>* current = head;
	while (current != nullptr)
	{
		cout << current->data << endl;
		current = current->next;
	}
}

#include "MyRwLock.h"

RWData<map<int, bool>> testRWData;


class test1
{
public:
	int a;
	string b;

public:
	test1()
	{
		a = 78;
		cout << "构造1" << endl;
	}

	test1(test1& a)
	{
		cout << "复制构造" << endl;
	}

	//test1(int _a = 1, string _b = "") :a(_a), b(_b) {
	//	cout << "构造" << endl;
	//}
};

void funt1(shared_ptr<test1> param)
{
	//param.a = 99;
	cout << "funt1" << endl;
}

DWORD WINAPI FunProc(LPVOID lpParameter)
{
	while (true)
	{
		cout << "xxsd" << endl;
	}

	return 0;
}

//template<class T, class A = allocator<T> >
//class vectorAny
//{
//public:
//	typedef typename A::value_type value_type;
//	typedef typename A::reference reference;
//	typedef A allocator_type;
//	typedef typename A::pointer pointer;
//	typedef typename A::size_type size_type;
//	// ...
//	explicit vector() throw() : sz(0), random_ptr() { }
//	// ...
//	void push_back(const value_type& v) {
//		!random_ptr ? random_ptr = new T(v) : new(random_ptr + sz) T(v);
//		sz++;
//	}
//
//	reference operator[](size_type ind) { return *(random_ptr + ind); }
//	size_type size() const { return sz; }
//
//private:
//	size_type sz;
//	pointer random_ptr;
//	// ...
//
//};



#include<any>

template<typename T>
class AnyStruct
{

};



int main() 
{
	any n_A = 4;
	any s_B = string("sdsd");
	any f_C = 3.9989;

	auto sdsd1 = f_C.type().name();

	using dsds = decltype(n_A._Cast<int>());

	std::map<string, any> ts;
	ts.insert(std::make_pair("cxx", 2));
	ts.insert(std::make_pair("c1xx", "xxs"));

	for (auto& element : ts)
	{
		if (element.second.type()==typeid(int))
		{
			auto s = element.second._Cast<int>();
			using typeT = decltype(s);
		}
	}
		
	int num = 21;

	std::vector<any> test;

	std::pair<string, any> tPair;

	auto func = [](auto s)
	{
		cout << s << endl;
	};

	//func(6);
	//func("sdsd");

	//testF(func);

	//AnyVectorTraversal(test, [](any sd){});

	test.push_back(n_A);
	test.push_back(s_B);
	test.push_back(f_C);

	decltype(num) ss = 213123;

	auto www = n_A._Cast<int>();
	auto tesq = n_A.type().name();
	auto s = typeid(int).name();

	tools::anyVectorTraverser(test, [](auto& value) {
		cout << typeid(value).name() << " ";
		cout << value << endl;
	});

	cout << endl;

	//char* szFilePath;
	//szFilePath = _getcwd(NULL, 0);

	//HANDLE m_thread = nullptr;

	////m_thread = CreateEvent(NULL, TRUE, FALSE, NULL);

	//m_thread = CreateThread(NULL, 0, FunProc, NULL, 0, NULL);

	//HANDLE m_event = nullptr;

	//m_event = CreateEvent(NULL, TRUE, FALSE, NULL);

	//SetEvent(m_event);

	//CloseHandle(m_event);

	//auto res = WaitForSingleObject(m_event, 2000);


	//if (WAIT_OBJECT_0 == res)
	//{
	//	cout << "ll" << endl;
	//}
	//else if (WAIT_TIMEOUT == res)
	//{
	//	cout << "l2l" << endl;
	//}
	//else if (WAIT_ABANDONED == res)
	//{
	//	cout << "ppp" << endl;
	//}

	//cout << endl;

	//std::list<int> testList;

	//testList.push_back(2);
	//testList.push_back(5);

	//auto it = testList.begin();

	//for (it; it != testList.end();it++)
	//{
	//	if (*it == 5)
	//	{
	//		*it = 9890;
	//		//testList.insert(it, 3);
	//	}
	//	//cout << *it << endl;
	//}



	//int a = 1;
	//int b = 0;

	//if (a)
	//{
	//	cout << "sss" << endl;
	//}

	//if (!b)
	//{
	//	cout << "sss" << endl;
	//}
	//auto t1 = make_shared<test1>();

	//funt1(std::move(t1));

	//cout << endl;

	//int index = 1;
	//Timer::schedule([&] {
	//	cout << "sdsda" << endl;
	//	for (int i = 0; i < 100; i++)
	//	{
	//		cout << index << " " << i << endl;
	//	}
	//	index++;
	//}, 0.5);

	//getchar();
	//std::list<int> m_listSimulate;
	//m_listSimulate.push_back(1);
	//m_listSimulate.push_back(2);
	//m_listSimulate.push_back(3);
	//m_listSimulate.push_back(2);

	//for (auto it = m_listSimulate.begin(); it != m_listSimulate.end();)
	//{
	//	if ((*it) == 2)
	//	{
	//		m_listSimulate.erase(it++);
	//	}
	//	else
	//		it++;
	//}

	//for (auto& element : m_listSimulate)
	//{
	//	if (2 == element)
	//	{
	//		m_listSimulate.remove(element);
	//	}
	//}


	//int test = 20210513;
	//auto timeLast = tools::ConvertIntDateToStamp(test);
	//auto timeNew = tools::ConvertIntDateToStamp(20210515);

	//auto diffSeconds = difftime(timeNew, timeLast);
	//auto diffDay = (int)diffSeconds / 60 / 60 / 24;

	//cout << "" << endl;

	//std::map<int, int> testMap;
	//testMap.insert(std::make_pair(20210513, 2));
	//testMap.insert(std::make_pair(20210514, 3));
	//testMap.insert(std::make_pair(20210515, 4));


	//std::stringstream ss2;
	//for (auto it = testMap.begin(); it != testMap.end(); it++)
	//{
	//	if (it != testMap.begin())
	//	{
	//		ss2 << ",";
	//	}
	//	ss2 << it->first << ":" << it->second;
	//}

	//std::string ttt = ss2.str();

	//cout << endl;

	//std::string info = "20210513:2,20210514:3,20210515:4";

	//std::stringstream ss("");
	//std::string s1;
	//while (std::getline(ss, s1, ','))
	//{
	//	auto pos = s1.find(':');
	//	auto date = atoi(s1.substr(0, pos).c_str());
	//	auto count = atoi(s1.substr(pos + 1, s1.size()).c_str());

	//	cout << count;
	//}

	//{
	//	//decltype(testRWData.getData())* targetData2 = nullptr;
	//	//map<int, bool>* targetData2 = nullptr;
	//	//auto writeGuard = testRWData.write(targetData2);
	//	RWDATA_WRITE(testRWData, targetData2);
	//	targetData2->insert({ 1,true });
	//	targetData2->insert({ 2,false });
	//	targetData2->insert({ 3,true });
	//}

	//{
	//	RWDATA_READ(testRWData, targetData);

	//	for (auto& it : *targetData)
	//	{
	//		cout << it.first << " " << it.second << endl;
	//	}
	//}

	//testFun1(3, 4, 5, 6);

	//tools::processArgs([](auto param) {
	//	cout << param << endl;
	//}, 9, 2, 4, 5);

	//auto& myPool = ThreadPool::getInstance();
	//RWLock testLock;

	//myAsync<void>([&]() {
	//	auto guard = testLock.Read();
	//	for (int i = 0; i < 10; i++)
	//	{
	//		this_thread::sleep_for(std::chrono::seconds(1));
	//		cout << this_thread::get_id() << endl;
	//	}
	//});

	//

	//myAsync<void>([&]() {
	//	//testLock.Read();
	//	this_thread::sleep_for(std::chrono::seconds(1));
	//	auto guard = testLock.Write();
	//	for (int i = 0; i < 10; i++)
	//	{
	//		this_thread::sleep_for(std::chrono::seconds(1));
	//		cout << this_thread::get_id() << endl;
	//	}
	//});

	//myPool.waitAllTaskFinish();

	//time_t now = std::time(NULL);
	//struct tm* tNow = std::localtime(&now);

	//struct tm t1 {};
	//t1.tm_year = tNow->tm_year - 1;
	//t1.tm_mon = 5;
	//t1.tm_mday = 22;
	//t1.tm_hour = 6;

	//time_t res = mktime(&t1);

	//res += 200 * 24 * 60 * 60;

	//auto tm2 = std::gmtime(&res);



	//cout << "sss" << endl;


	//std::shared_mutex sharedMutex;
	//std::shared_lock<std::shared_mutex>(sharedMutex);



	//vector<int> test{ 10, 80, 10 };
	//std::default_random_engine m_engine;
	//std::uniform_int_distribution<int> m_distribution(0, 100);

	//for (int j = 0; j < 100; j++)
	//{
	//	auto res = m_distribution(m_engine);
	//	int sum = 0;
	//	for (int i = 0; i < test.size(); i++)
	//	{
	//		sum += test[i];
	//		if (res <= sum)
	//		{
	//			cout << i << endl;
	//			break;
	//		}
	//	}
	//}

	//int a = 0x8000;
	//int b = 0x800f;

	//int c = a & b;



	//auto testStr = tools::ReadFileToString("test\\testFile1.txt");

	//cout << testStr << endl;

	
	//Node<int> n1, n2, n3, n4;
	//n1.data = 9;
	//n2.data = 8;
	//n3.data = 7;
	//n4.data = 6;
	//n1.next = &n2;
	//n2.next = &n3;
	//n3.next = &n4;
	//n4.next = nullptr;
	//printNum(&n1);

	//

	//flip(&n1);
	//printNum(&n4);

	//int s = 2;

	//++s = 3;

	////sssss = 1;
	//std::vector<int> v1{ 5,3,1,8,4,2,9 };
	////Algorithm::sort1(v1);

	//Algorithm::sortT1(v1);

	//for (auto& item : v1)
	//{
	//	cout << item << endl;
	//}


	//std::array<int, 10> testA{ 5,5,3,12 };

	//int* a[10];
	//int(*p)[20];
	
	//CTime s(0, 0, 0, 0, 0, 0);

	//CTime time2(2020, 12, 23, 0, 0, 0);

	//CTimeSpan tsp = s - time2;

	//auto res = abs(tsp.GetDays());

	return 0;
}