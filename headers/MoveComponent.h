#pragma once
#include "Component.h"
#include "EnemyActor.h"

class MoveComponent : public Component
{
public:
	
	MoveComponent(class Actor* newOwner, int updateOrder = 10);
	void Update(float deltaTime) override;

	void Collided();
	void UpdatePosition();
	void StairsFound();

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

	bool movingUp;
	bool movingDown;
	bool movingLeft;
	bool movingRight;

private:
	float horizontalMove;
	float verticalMove;
	Game* game;

	float dest;
	float speed;
	Vector3 dest_pos;
};