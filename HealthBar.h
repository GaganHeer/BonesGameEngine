#pragma once

#include "HudELement.h"

class HealthBar : public HudElement
{
public:
	const char* ASSET_BG = "Assets/healthbar_bg.png";
	const char* ASSET_FG = "Assets/healthbar_fg2.png";

	HealthBar(Actor* actor, Vector3 pos, int minValue, int maxValue);
	~HealthBar();

	void update(int value);
	void updateRange(int min, int max);
	int getRange() const;

	void SetPosition(Vector3 pos);

private:
	Actor* actorFG;
	SpriteComponent* sc_FG;
	Texture* textureBG;
	Texture* textureFG;
	int minRange;
	int maxRange;
	int currentValue;
	int textureFG_width;
	int textureBG_width;
	float textureFG_curScale;

	void loadTexture();
};

