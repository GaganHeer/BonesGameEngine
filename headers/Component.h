#pragma once
#include <cstdint>
#include "InputSystem.h"
#include "Actor.h"

class Component
{
public:
	// Constructor
	Component(class Actor* owner, int updateOrder = 100);
	// Destructor
	virtual ~Component();
	virtual void Update(float deltaTime);
	virtual void ProcessInput(InputState keyState) {};
	virtual void OnUpdateWorldTransform() { }

	int GetUpdateOrder() const { 
		return updateOrder; 
	}

	class Actor* GetOwner() {
		return owner;
	}

protected:
	class Actor* owner;
	int updateOrder;
};
