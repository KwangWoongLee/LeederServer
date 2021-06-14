#pragma once
#include "stdafx.h"


class InputManager : public Singleton<InputManager>
{
public:
	bool Init();
	void PrepareForUpdate();
	const KeyboardState& GetKeyboardState() const { return mKeyboardState; }

	void HandleInput(const class KeyboardState& keyState);

	std::deque<Input>& GetInputList() { return mInputList; }
	void	RemoveProcessInput(float serverLastProcessTime);

private:
	KeyboardState mKeyboardState;
	std::deque<Input> mInputList;

};
