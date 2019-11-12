#pragma once

#include "HudElement.h"

class HUD
{
	public:
		HUD();
		~HUD();

		void addElement(HudElement* hudElement);
		void removeElement(HudElement* hudElement);
		void clearHUD();

	private:
		std::vector<HudElement*> hud_elements;
};

