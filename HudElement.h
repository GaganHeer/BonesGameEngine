#pragma once

#include "Actor.h"
#include "SpriteComponent.h"
#include "Texture.h"

class HudElement
{
	public:
		HudElement();
		HudElement(Actor* anActor, Vector3 pos);
		virtual ~HudElement();

		virtual void SetPosition(Vector3 pos);
		Vector3 GetPosition() const { return pos; };

	protected:
		Actor* element;
		Vector3 pos;
		SpriteComponent* sc;
};

