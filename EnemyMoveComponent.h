#pragma once
#include "Component.h"
#include "EnemyActor.h"
#include <utility>

class EnemyMoveComponent : public Component
{
public:
	EnemyMoveComponent(class Actor* newOwner, int updateOrder = 10);
	void Update(float deltaTime) override;
	void MoveEnemy();

	enum EnemyState {
		WANDER,
		CHASE
	};

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

	void Collided();

private:
	class Game* game;
	bool move;
	bool moveable;

	bool movingUp;
	bool movingDown;
	bool movingLeft;
	bool movingRight;

	float dest;
	float speed;
	Vector3 dest_pos;
	EnemyState state;
	int** map2D;

	bool spotted;
};

