#include "stdafx.h"
#include "InputManager.h"


bool InputManager::Init()
{
	mKeyboardState.mCurrState = SDL_GetKeyboardState(NULL);
	// Clear previous state memory
	memset(mKeyboardState.mPrevState, 0,
		SDL_NUM_SCANCODES);


	return true;
}

void InputManager::PrepareForUpdate()
{
	memcpy(mKeyboardState.mPrevState,
		mKeyboardState.mCurrState,
		SDL_NUM_SCANCODES);
}

void InputManager::HandleInput(const class KeyboardState& keyState)
{
	eInputType type = InputFactory::GetInstance().GetInputType(keyState);

	if (type != eInputType::NONE)
		mInputList.push_back(Input(type));
}

void InputManager::RemoveProcessInput(float serverLastProcessTime)
{
	while (!mInputList.empty() && mInputList.front().GetTimeStamp() <= serverLastProcessTime)
	{
		mInputList.pop_front();
	}


}

