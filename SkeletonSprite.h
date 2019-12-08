#pragma once
#include "Actor.h"
#include "SpriteComponent.h"

class SkeletonSprite : public Actor
{
public:
	enum SpriteStates
	{
		Attacking,
		Idle,
		Dying,
		Dodging
	};
	SkeletonSprite(class Game* game);
	void UpdateActor(float deltaTime) override;

	void SwitchState(SpriteStates s);
	SpriteStates state;
	bool ready;
private:
	int tileNum;
	float time;
	SpriteComponent* sc;
};

