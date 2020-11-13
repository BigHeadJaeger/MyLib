#pragma once
#include<initializer_list>

namespace tools
{
	template<typename F, typename... Args>
	void processArgs(F processFunc, Args&&... args)
	{
		std::initializer_list<int>({ (processFunc(std::forward<Args>(args)), 0) ... });
	}
}


