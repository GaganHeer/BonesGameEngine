#include "InputSystem.h"
#include <SDL.h>
#include <cstring>

//return true if the current state of keys is being pressed
bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const{
	return currState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const{
	if (prevState[keyCode] == 0){
		if (currState[keyCode] == 0){
			//If the previous state is 0 and the current state is 0 then no button was ever pressed
			return ButtonState::None;
		}
		else {
			//If the previous state is 0 and the current state is 1 then some button was just pressed
			return ButtonState::Pressed;
		}
	}
	else {
		if (currState[keyCode] == 0){
			//If the previous state is 1 and the current state is 0 then some button was just released
			return ButtonState::Released;
		}
		else {
			//If the previous state is 1 and the current state is 1 then some button is being held down
			return ButtonState::Held;
		}
	}
}

bool InputSystem::Initialize(){
	// Assign current state pointer
	state.Keyboard.currState = SDL_GetKeyboardState(NULL);
	// Clear previous state memory
	memset(state.Keyboard.prevState, 0, SDL_NUM_SCANCODES);
	return true;
}

// Called before SDL_PollEvents loop
void InputSystem::BeforeUpdate(){
	// Copy current state to previous
	memcpy(state.Keyboard.prevState, state.Keyboard.currState, SDL_NUM_SCANCODES);
}

// Called after SDL_PollEvents loop
void InputSystem::Update(){
	//Perform update actions
}