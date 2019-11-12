#pragma once
#include "Actor.h"
#include "InputSystem.h"

class EnemyActor : public Actor
{
public:
	EnemyActor(class Game* game);
	void ActorInput(const InputState keyState) override;
	void SetVisible(bool visible);

	void SetMoveable(bool value) {
		moveable = value;
	}
private:
	class EnemyMoveComponent* enemyMoveComponent;
	class MeshComponent* meshComponent;
	class SkeletalMeshComponent* skeletalMeshComponent;

	bool moveable;
};