#include "stdafx.h"
#include "InputManager.h"


bool InputManager::Init()
{
	mKeyboardState.mCurrState = SDL_GetKeyboardState(NULL);
	// Clear previous state memory
	memset(mKeyboardState.mPrevState, 0,
		SDL_NUM_SCANCODES);

	inputCount = 0;

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
	auto type = InputFactory::GetInstance().GetInputType(keyState);

	//auto type = (eInputType)(GetRandomInt()%6);

	if (type != eInputType::NONE)
		mInputList.push_back(type);
}

eInputType InputManager::PopInputList()
{
	auto type = mInputList.front();
	mInputList.pop_front();
	return type;
}

