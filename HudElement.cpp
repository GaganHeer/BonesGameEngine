#include "HudElement.h"

HudElement::HudElement()
{

}

HudElement::HudElement(Actor* anActor, Vector3 pos, std::string data)
{
	element = anActor;
	this->pos = pos;
	this->data = data;

	sc = new SpriteComponent(anActor);
	fontTexture = nullptr;
	fontRenderer = new Font();
	fontRenderer->Load("Assets/Carlito-Regular.ttf");
	element->SetPosition(pos);
}

void HudElement::UpdateText(const std::string& text)
{
	if (text.length() > 0)
	{
		data = text;
		if (fontTexture != nullptr) fontTexture->Unload();

		fontTexture = fontRenderer->RenderText(text.c_str(), Color::LightYellow, Color::LightBlue, Font::LARGE_FONT_3, true);
		sc->SetTexture(fontTexture);
	}
}

void HudElement::SetPosition(Vector3 pos)
{
	element->SetPosition(pos);
}

HudElement::~HudElement()
{
	if (fontTexture != NULL) fontTexture->Unload();
	delete fontRenderer;
	delete sc;
	delete element;
}