#pragma once

#include "HudElement.h"
#include "TextBox.h"
#include "HealthBar.h"
#include "Actor.h"

class HUD
{
	public:
		enum ElementType
		{
			TEXT_BOX,
			HEALTH_BAR,
			DIALOG_BOX
		};

		HUD();
		~HUD();

		HudElement* addElement(Actor* actor, ElementType type);
		void removeElement(HudElement* hudElement);
		void clearHUD();
		bool isElementInHUD(HudElement* hudElement);

	private:
		std::vector<HudElement*> hud_elements;
};

