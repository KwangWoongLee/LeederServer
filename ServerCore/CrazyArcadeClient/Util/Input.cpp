#include "stdafx.h"
#include "Input.h"

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
	return mCurrState[keyCode] == 1;
}

eButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
	if (mPrevState[keyCode] == 0)
	{
		if (mCurrState[keyCode] == 0)
		{
			return eButtonState::NONE;
		}
		else
		{
			return eButtonState::PRESSED;
		}
	}
	else // Prev state must be 1
	{
		if (mCurrState[keyCode] == 0)
		{
			return eButtonState::RELEASED;
		}
		else
		{
			return eButtonState::HELD;
		}
	}
}
