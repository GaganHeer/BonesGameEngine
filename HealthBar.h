#pragma once

#include "HudELement.h"

class HealthBar : public HudElement
{
public:
	enum Type
	{
		RED_BAR,
		GREEN_BAR
	};

	const char* ASSET_BG = "Assets/healthbar_bg.png";
	const char* ASSET_FG_RED = "Assets/healthbar_red.png";
	const char* ASSET_FG_GREEN = "Assets/healthbar_green.png";

	HealthBar(Actor* actor, Vector3 pos, int minValue, int maxValue, Type type);
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
	Type type;

	int currentValue;
	int textureFG_width;
	int textureBG_width;
	float textureFG_curScale;

	void loadTexture();
};

