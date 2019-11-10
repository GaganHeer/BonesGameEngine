#pragma once

#include <string>
#include <unordered_map>
#include <SDL_ttf.h>
#include "Math.h"

class Font

{

public:

	Font();
	~Font();

	// Load/unload from a file
	bool Load(const std::string& fileName);
	void Unload();

	// Given string and this font, draw to a texture
	class Texture* RenderText(const std::string& textMessage, const Vector3& color = Color::White, int pointSize = 24, bool blend = true);

private:
	// Map of point sizes to font data
	std::unordered_map<int, TTF_Font*> mFontData;
};

