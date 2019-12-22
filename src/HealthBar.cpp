#include "HealthBar.h"

HealthBar::HealthBar(Actor* actor, Vector3 pos, int minValue, int maxValue, Type type) : HudElement(actor, pos)
{
	textureBG = nullptr;
	textureFG = nullptr;
	minRange = minValue;
	maxRange = maxValue;
	currentValue = minValue;
	this->type = type;

	actorFG = new Actor(actor->GetGame());
	sc_FG = new SpriteComponent(actorFG);

	loadTexture();
}

HealthBar::~HealthBar()
{
	if (textureBG != nullptr) textureBG->Unload();
	if (textureFG != nullptr) textureFG->Unload();

	delete sc_FG;
	delete actorFG;
}

void HealthBar::SetPosition(Vector3 pos)
{
	HudElement::SetPosition(pos);
	actorFG->SetPosition(pos);
}

void HealthBar::update(int value)
{
	currentValue = value;
	if (currentValue < 0) currentValue = 0;

	textureFG_curScale = (float)currentValue / getRange();
	actorFG->SetScaleV(Vector3(textureFG_curScale, 1.0f, 1.0f));

	// following is to adjust FB.x to align with BG.x
	// we wanna scale FG based on BG
	float scaledWidthFG = textureBG_width * textureFG_curScale;
	int newX = pos.x - (textureBG_width / 2);
	newX = newX + (scaledWidthFG / 2);

	actorFG->SetPosition(Vector3(newX, pos.y, pos.z));
}

void HealthBar::updateRange(int min, int max)
{
	minRange = min;
	maxRange = max;
}

int HealthBar::getRange() const
{
	return maxRange - minRange;
}

void HealthBar::loadTexture()
{
	textureBG = new Texture();
	textureBG->Load(ASSET_BG);

	textureFG = new Texture();
	switch (type)
	{
		case RED_BAR:
			textureFG->Load(ASSET_FG_RED);
		break;
		case GREEN_BAR:
		default:
			textureFG->Load(ASSET_FG_GREEN);
			break;
	}
	

	textureFG_width = textureFG->GetWidth();
	textureBG_width = textureBG->GetWidth();

	sc->SetTexture(textureBG);
	sc_FG->SetTexture(textureFG);
	actorFG->SetPosition(pos);
}