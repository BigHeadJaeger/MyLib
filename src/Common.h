#pragma once
#include<initializer_list>
#include<istream>
#include<fstream>
#include<sstream>
#include<any>

namespace tools
{
	// 对函数中可变参数列表进行逐一处理(处理函数，参数列表)
	template<typename F, typename... Args>
	void processArgs(F processFunc, Args&&... args)
	{
		std::initializer_list<int>({ (processFunc(std::forward<Args>(args)), 0) ... });
	}

	// 对any类型的容器进行遍历(目标容器，处理函数)
	template<typename F>
	void anyVectorTraverser(std::vector<any>& target, F func)
	{
		for (auto& element : target)
		{
			if (typeid(int) == element.type()) { func(*element._Cast<int>()); }
			else if (typeid(string) == element.type()) { func(*element._Cast<string>()); }
			else if (typeid(char) == element.type()) { func(*element._Cast<char>()); }
			else if (typeid(float) == element.type()) { func(*element._Cast<float>()); }
		}
	}


	// -------------------------------- 文件相关start --------------------------------

	// 将文件读取为字符串
	std::string ReadFileToString(const std::string fileName)
	{
		std::string strBuffer;
		std::ifstream reader(fileName);
		if (reader)
		{
			std::stringstream sstream;
			sstream << reader.rdbuf();
			strBuffer = sstream.str();
		}
		
		return strBuffer;
	}

	// 获取文件的最后修改时间


	// -------------------------------- 文件相关end --------------------------------


	// -------------------------------- 时间计算start --------------------------------
	// 将数字格式的日期转为时间戳(yyyymmdd --> stamp)
	time_t ConvertIntDateToStamp(int nDate)
	{
		int nyear = nDate / 10000;
		int nmonth = (nDate % 10000) / 100;
		int nday = (nDate % 10000) % 100;

		tm info = { 0 };
		info.tm_year = nyear - 1900;
		info.tm_mon = nmonth - 1;
		info.tm_mday = nday;
		return std::mktime(&info);
	}

	// 获取现在时间(yyyymmdd)
	int GetDateTime()
	{
		time_t now = std::time(NULL);
		tm* tNow = std::localtime(&now);

		int nDate = (tNow->tm_year + 1900) * 10000 +
			(tNow->tm_mon + 1) * 100 +
			tNow->tm_mday;

		return nDate;
	}

	// -------------------------------- 时间计算end --------------------------------


}


