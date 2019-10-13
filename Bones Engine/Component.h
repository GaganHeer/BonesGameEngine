#pragma once
#include <cstdint>
#include "InputSystem.h"

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	virtual void Update(float deltaTime);
	virtual void ProcessInput(InputState keyState) {};
	virtual void OnUpdateWorldTransform() { }
	int GetUpdateOrder() const {
		return updateOrder;
	}
protected:
	int updateOrder;
	class Actor* owner;
};

