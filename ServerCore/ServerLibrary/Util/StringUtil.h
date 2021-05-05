#pragma once
#include "stdafx.h"

namespace leeder
{

//const char* to wstring
inline std::wstring CA2W(IN CONST CHAR* str) { ATL::CA2W ca2w(str); return std::wstring(ca2w); }

//const char* to string
inline std::string  CW2A(IN CONST WCHAR* str) { ATL::CW2A cw2a(str); return std::string(cw2a); }



};