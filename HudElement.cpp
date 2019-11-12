#include "HudElement.h"

HudElement::HudElement(Actor* anActor, Vector3 pos, Vector2 size, string* data)
{
	element = anActor;
	this->pos = pos;
	this->size = size;
	this->data = data;

	sc = new SpriteComponent(element);

	fontRenderer = new Font();
	fontRenderer->Load("Assets/Carlito-Regular.ttf");
	
	element->SetPosition(pos);
	fontTexture = fontRenderer->RenderText(data->c_str(), Color::LightYellow, Color::LightBlue, Font::LARGE_FONT_3, true);
	sc->SetTexture(fontTexture);
}

void HudElement::UpdateText(const std::string& text)
{
	if (data == nullptr || data->compare(text) != 0)
	{
		delete fontTexture;
		fontTexture = fontRenderer->RenderText(text.c_str(), Color::LightYellow, Color::LightBlue, Font::LARGE_FONT_3, true);
		sc->SetTexture(fontTexture);
	}
}

HudElement::~HudElement()
{
	delete data;
	if (fontTexture) delete fontTexture;
	if (fontRenderer) delete fontRenderer;
	//if (sc) delete sc;
	//if (element) delete element;
}