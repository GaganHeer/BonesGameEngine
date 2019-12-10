#include "HudElement.h"

HudElement::HudElement()
{
	element = nullptr;
	sc = nullptr;
	pos = Vector3();
}

HudElement::HudElement(Actor* anActor, Vector3 pos)
{
	element = anActor;
	this->pos = pos;

	sc = new SpriteComponent(anActor);
	element->SetPosition(pos);
}

void HudElement::SetPosition(Vector3 pos)
{
	this->pos = pos;
	element->SetPosition(pos);
}

HudElement::~HudElement()
{
	delete sc;
	delete element;
}