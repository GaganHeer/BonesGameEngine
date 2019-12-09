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

		void SetPosition(Vector3 pos);

	protected:
		Actor* element;
		Vector3 pos;
		SpriteComponent* sc;
};

