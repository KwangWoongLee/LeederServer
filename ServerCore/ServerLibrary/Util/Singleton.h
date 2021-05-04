#pragma once
#include "stdafx.h"

// Singleton 클래스 정의
// volatile int dummy{}; 를 포함시켜 최적화 방지
// Singleton 클래스를 상속 받아 프로그램 내의 단 하나 객체만 생성/사용
//
// Example:
//
// 정의 예
// class Test : public Singleton< Test >
// {
// }
//
//
// 사용 예 
// Test::GetInstance();

namespace leeder
{

template <typename T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T m_pInstance;
		volatile int dummy{};

		return m_pInstance;
	}

protected:
	Singleton() = default;
	~Singleton() = default;
};

};