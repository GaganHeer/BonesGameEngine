#pragma once
#include "Actor.h"

class CubeActor : public Actor
{
public:
	CubeActor(class Game* game);

	void UpdateActor(float deltaTime) override;

};
