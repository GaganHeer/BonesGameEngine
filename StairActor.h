#pragma once
#include "Actor.h"

class StairActor : public Actor
{
public:
	StairActor(class Game* game);

	void UpdateActor(float deltaTime) override;

};
