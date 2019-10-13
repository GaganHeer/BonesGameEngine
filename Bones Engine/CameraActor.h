#pragma once
#include "Actor.h"

class CameraActor : public Actor {
public:
	CameraActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(InputState keys) override;
private:
	class MoveComponent* moveComp;
};