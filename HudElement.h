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
		HudElement();
		HudElement(Actor* anActor, Vector3 pos, std::string data);
		~HudElement();

		void UpdateText(const std::string& text);
		void SetPosition(Vector3 pos);

	private:
		Actor* element;
		Vector3 pos;
		SpriteComponent* sc;
		std::string data;
		Font* fontRenderer;
		Texture* fontTexture;
};

