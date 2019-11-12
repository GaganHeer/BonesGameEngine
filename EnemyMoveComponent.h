#pragma once
#include "Component.h"
#include "EnemyActor.h"

class EnemyMoveComponent : public Component
{
public:
	EnemyMoveComponent(class Actor* newOwner, int updateOrder = 10);
	void Update(float deltaTime) override;
	void MoveEnemy();

	float GetMove() const {
		return move;
	}

	void SetMove(bool value) {
		move = value;
	}

	float GetMoveable() const {
		return moveable;
	}

	void SetMoveable(bool value) {
		moveable = value;
	}

private:
	class Game* game;
	bool move;
	bool moveable;
};

