#pragma once
#include "stdafx.h"


class InputManager : public Singleton<InputManager>
{
public:
	bool Init();
	void PrepareForUpdate();
	const KeyboardState& GetKeyboardState() const { return mKeyboardState; }

	void HandleInput(const class KeyboardState& keyState);

	std::list<eInputType>& GetInputList() { return mInputList; }
	eInputType	PopInputList();

	int inputCount;


private:
	KeyboardState mKeyboardState;
	std::list<eInputType> mInputList;

};
