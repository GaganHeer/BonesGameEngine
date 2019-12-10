#include "HUD.h"

HUD::HUD()
{
	
}

HUD::~HUD()
{
	clearHUD();
}

HudElement* HUD::addElement(Actor* actor, ElementType type)
{
	switch (type)
	{
	case HEALTH_BAR_RED:
		hud_elements.push_back(new HealthBar(actor, Vector3(), 0, 0, HealthBar::RED_BAR));
		break;
	case HEALTH_BAR_GREEN:
		hud_elements.push_back(new HealthBar(actor, Vector3(), 0, 0, HealthBar::GREEN_BAR));
		break;
	case TEXT_BOX:
	default:
		hud_elements.push_back(new TextBox(actor, Vector3(), ""));
		break;
	}
	return hud_elements.back();
}

void HUD::removeElement(HudElement* hudElement)
{
	vector<HudElement*>::iterator itr;
	for (itr = hud_elements.begin(); itr < hud_elements.end(); itr++)
	{
		if ((*itr) == hudElement)
		{
			hud_elements.erase(itr);
			delete hudElement;
			break;
		}
	}
}

bool HUD::isElementInHUD(HudElement* hudElement)
{
	vector<HudElement*>::iterator itr;
	for (itr = hud_elements.begin(); itr < hud_elements.end(); itr++)
	{
		if ((*itr) == hudElement)
		{
			return true;
		}
	}

	return false;
}

void HUD::clearHUD()
{
	for (int i = 0; i < hud_elements.size(); i++)
	{
		delete hud_elements[i];
	}
	hud_elements.clear();
}

