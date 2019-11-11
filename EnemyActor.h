#pragma once
#include "Actor.h"
#include "InputSystem.h"

class EnemyActor : public Actor
{
public:
	EnemyActor(class Game* game);

	void ActorInput(const InputState keyState) override;

	void SetVisible(bool visible);
private:
	class EnemyMoveComponent* enemyMoveComponent;
	class MeshComponent* meshComponent;
};