#pragma once

#include <string>
#include "Font.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "Math.h"

class HudElement
{
	public:
		HudElement(Actor* anActor, Vector3 pos, Vector2 size, string* data);
		~HudElement();

		void UpdateText(const std::string& text);

	private:
		Actor* element;
		Vector3 pos;
		SpriteComponent* sc;
		Vector2 size;
		string* data;
		Font* fontRenderer;
		Texture* fontTexture;
};

