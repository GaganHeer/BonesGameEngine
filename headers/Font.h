#pragma once

#include <string>
#include <unordered_map>
#include <SDL_ttf.h>
#include <assert.h>
#include "MathT.h"

class Font
{
	public:
		// We support these font sizes
		enum FONT_SIZE { 
			TINY_FONT_1 = 6,
			TINY_FONT_2 = 8,
			TINY_FONT_3 = 10,
			SMALL_FONT_1 = 12,
			SMALL_FONT_2 = 14,
			SMALL_FONT_3 = 18,
			MEDIUM_FONT_1 = 22,
			MEDIUM_FONT_2 = 24,
			MEDIUM_FONT_3 = 28,
			BIG_FONT_1 = 32,
			BIG_FONT_2 = 34,
			BIG_FONT_3 = 38,
			LARGE_FONT_1 = 42,
			LARGE_FONT_2 = 44,
			LARGE_FONT_3 = 48,
			XL_FONT_1 = 52,
			XL_FONT_2 = 62,
			XL_FONT_3 = 72
		};

		Font();
		~Font();

		// Load/unload from a file
		bool Load(const std::string& fileName);
		void Unload();

		// Given string and this font, draw to a texture
		class Texture* RenderText(const std::string& textMessage, const Vector3& color_fg = Color::White,
								const Vector3& color_bg = Color::Black, int pointSize = MEDIUM_FONT_2, bool blend = true);

	private:
		// Map of point sizes to font data
		std::unordered_map<int, TTF_Font*> fontData;
};

