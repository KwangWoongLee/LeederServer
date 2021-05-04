#pragma once
#include "stdafx.h"

// Singleton Ŭ���� ����
// volatile int dummy{}; �� ���Խ��� ����ȭ ����
// Singleton Ŭ������ ��� �޾� ���α׷� ���� �� �ϳ� ��ü�� ����/���
//
// Example:
//
// ���� ��
// class Test : public Singleton< Test >
// {
// }
//
//
// ��� �� 
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