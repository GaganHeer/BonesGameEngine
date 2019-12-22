#pragma once
#include "Actor.h"
#include "SpriteComponent.h"

class VictorySprite : public Actor
{
public:
	VictorySprite(class Game* game);
	void UpdateActor(float deltaTime) override;
	bool ready;
private:
	int tileNum;
	float time;
	SpriteComponent* sc;
};

