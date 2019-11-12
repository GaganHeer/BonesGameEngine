#include "Font.h"
#include "Texture.h"

Font::Font()
{

}

Font::~Font()
{
	Unload();
}

bool Font::Load(const std::string& fileName)
{
	std::vector<int> fontSizes = {
		TINY_FONT_1, TINY_FONT_2, TINY_FONT_3,
		SMALL_FONT_1, SMALL_FONT_2, SMALL_FONT_3,
		MEDIUM_FONT_1, MEDIUM_FONT_2, MEDIUM_FONT_3,
		BIG_FONT_1, BIG_FONT_2, BIG_FONT_3,
		LARGE_FONT_1, LARGE_FONT_2, LARGE_FONT_3,
		XL_FONT_1, XL_FONT_2, XL_FONT_3
	};

	for (int size = 0; size < fontSizes.size(); size++)
	{
		TTF_Font* font = TTF_OpenFont(fileName.c_str(), fontSizes[size]);
		if (font == nullptr)
		{
			SDL_Log("Failed to load font %s in size %d", fileName.c_str(), fontSizes[size]);
			SDL_Log("Failure reason %s", TTF_GetError());
			return false;
		}
		fontData.emplace(fontSizes[size], font);
	}
	return true;
}

void Font::Unload()
{
	for (auto& font : fontData)
	{
		TTF_CloseFont(font.second);
	}
}

Texture* Font::RenderText(const std::string& textMessage, const Vector3& color_fg, const Vector3& color_bg, int pointSize, bool blend)
{
	Texture* texture = nullptr;

	// Convert to SDL_Color
	SDL_Color sdlColor_fg;
	sdlColor_fg.r = static_cast<Uint8>(color_fg.x * 255);
	sdlColor_fg.g = static_cast<Uint8>(color_fg.y * 255);
	sdlColor_fg.b = static_cast<Uint8>(color_fg.z * 255);
	sdlColor_fg.a = 255;

	// Convert to SDL_Color
	SDL_Color sdlColor_bg;
	sdlColor_bg.r = static_cast<Uint8>(color_bg.x * 255);
	sdlColor_bg.g = static_cast<Uint8>(color_bg.y * 255);
	sdlColor_bg.b = static_cast<Uint8>(color_bg.z * 255);
	sdlColor_bg.a = 255;

	// Find the font data for this point size
	auto iter = fontData.find(pointSize);

	if (iter != fontData.end())
	{
		TTF_Font* font = iter->second;

		// Draw this to a surface (blended for alpha
		SDL_Surface* messageSurf = nullptr;
		if (blend)
		{
			messageSurf = TTF_RenderUTF8_Blended(font, textMessage.c_str(), sdlColor_fg);
		}
		else
		{
			messageSurf = TTF_RenderUTF8_Shaded(font, textMessage.c_str(), sdlColor_fg, sdlColor_bg);
		}

		assert(messageSurf != nullptr);

		if (messageSurf != nullptr)
		{
			// Convert from surface to texture
			texture = new Texture();
			texture->CreateFromSurface(messageSurf);
			SDL_FreeSurface(messageSurf);
		}
	}
	else
	{
		SDL_Log("Point size %d is unsupported", pointSize);
	}

	return texture;
}