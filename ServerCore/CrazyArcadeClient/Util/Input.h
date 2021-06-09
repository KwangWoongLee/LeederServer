#pragma once
#include "stdafx.h"


enum class eButtonState
{
	NONE,
	PRESSED,
	RELEASED,
	HELD
};


class KeyboardState
{
public:
	friend class InputManager;
	// Get just the boolean true/false value of key
	bool GetKeyValue(SDL_Scancode keyCode) const;
	// Get a state based on current and previous frame
	eButtonState GetKeyState(SDL_Scancode keyCode) const;
private:
	const Uint8* mCurrState;
	Uint8 mPrevState[SDL_NUM_SCANCODES];
};



class InputFactory : public Singleton<InputFactory>
{
public:
	eInputType GetInputType(KeyboardState state)
	{
		if (state.GetKeyValue(SDL_SCANCODE_SPACE))
		{
			return eInputType::SPACE;
		}

		else if (state.GetKeyValue(SDL_SCANCODE_A))
		{
			return eInputType::A;
		}

		else if (state.GetKeyValue(SDL_SCANCODE_S))
		{
			return eInputType::S;
		}

		else if (state.GetKeyValue(SDL_SCANCODE_D))
		{
			return eInputType::D;
		}

		else if (state.GetKeyValue(SDL_SCANCODE_W))
		{
			return eInputType::W;
		}

		else
		{
			return eInputType::NONE;
		}
	}

};