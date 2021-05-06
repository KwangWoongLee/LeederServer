#pragma once
#include "stdafx.h"

namespace leeder
{

#undef ASSERT
#define ASSERT(x)	            Assert(x, _T(#x), _T(__FILE__), __LINE__)				   				

	void Assert(bool condition, LPCWSTR conditionStr, LPCWSTR fileName, int lineNo);
	

};

