#include "HUD.h"

HUD::HUD()
{
	
}

HUD::~HUD()
{
	clearHUD();
}

void HUD::addElement(HudElement* hudElement)
{
	hud_elements.push_back(hudElement);
}

void HUD::removeElement(HudElement* hudElement)
{
	vector<HudElement*>::iterator itr;
	for (itr = hud_elements.begin(); itr < hud_elements.end(); itr++)
	{
		if ((*itr) == hudElement)
		{
			hud_elements.erase(itr);
			break;
		}
	}
}

void HUD::clearHUD()
{
	for (int i = 0; i < hud_elements.size(); i++)
	{
		delete hud_elements[i];
	}
	hud_elements.clear();
}

