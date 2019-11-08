#pragma once
#include "Component.h"
#include "Game.h"

class MoveComponent : public Component
{
public:
	
	MoveComponent(class Actor* newOwner, int updateOrder = 10);
	void Update(float deltaTime) override;
	
	float GetHorizontalMove() const {
		return horizontalMove;
	}

	float GetVerticalMove() const {
		return verticalMove;
	}

	void SetHorizontalMove(float move) { 
		horizontalMove = move;
	}

	void SetVerticalMove(float move) {
		verticalMove = move;
	}

private:
	float horizontalMove;
	float verticalMove;
	Game* game;
};