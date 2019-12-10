#include "TextBox.h"

TextBox::TextBox(Actor* anActor, Vector3 pos, std::string data) : HudElement(anActor, pos)
{
	this->data = data;
	fontTexture = nullptr;
	fontRenderer = new Font();
	fontRenderer->Load("Assets/Carlito-Regular.ttf");

	size = Font::LARGE_FONT_3;
	color = Color::LightYellow;
}

TextBox::~TextBox()
{
	if (fontTexture != nullptr) fontTexture->Unload();
	delete fontRenderer;
}

void TextBox::UpdateFontProperties(Vector3 color, int size)
{
	this->size = size;
	this->color = color;
}

void TextBox::UpdateText(const std::string& text)
{
	if (text.length() > 0)
	{
		data = text;
		if (fontTexture != nullptr) fontTexture->Unload();

		fontTexture = fontRenderer->RenderText(text.c_str(), color, Color::LightBlue, size, true);
		sc->SetTexture(fontTexture);
	}
}
