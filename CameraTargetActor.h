#pragma once
#include "Actor.h"
#include "InputSystem.h"

class CameraTargetActor : public Actor
{
public:
	CameraTargetActor(class Game* game);

	void ActorInput(const InputState keyState) override;

	void SetVisible(bool visible);
private:
	class MoveComponent* moveComponent;
	class MeshComponent* meshComponent;
	class TopDownCamera* cameraComponent;
	class SkeletalMeshComponent* skeletalMeshComponent;
	int dir;
	bool idle;
};

