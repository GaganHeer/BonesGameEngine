#pragma once

#include <string>
#include "Font.h"
#include "HudELement.h"

class TextBox : public HudElement
{
public:
	TextBox(Actor* anActor, Vector3 pos, std::string data);
	~TextBox();

	void UpdateFontProperties(Vector3 color, int size);
	void UpdateText(const std::string& text);

private:
	std::string data;
	Font* fontRenderer;
	Texture* fontTexture;
	Vector3 color;
	int size;
};