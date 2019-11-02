#pragma once
#include <iostream>
#include <SDL_scancode.h>
#include <assert.h>

enum class ButtonState {
	None,
	Pressed,
	Released,
	Held
};

class KeyboardState
{
public:
	friend class InputSystem;
	bool GetKeyValue(SDL_Scancode keyCode) const;
	ButtonState GetKeyState(SDL_Scancode keyCode) const;

private:
	const Uint8* currState;
	Uint8 prevState[SDL_NUM_SCANCODES];
};

struct InputState {
	KeyboardState Keyboard;
};

class InputSystem {
public:
	bool Initialize();
	void BeforeUpdate();
	void Update();

	const InputState& GetState() const {
		return state;
	}

private:
	InputState state;
};