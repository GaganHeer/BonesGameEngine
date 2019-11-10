#include "Font.h"
#include "Texture.h"
#include <vector>
#include "Game.h"

Font::Font()
{

}

Font::~Font()
{

}

bool Font::Load(const std::string& fileName)
{
	// We support these font sizes
	std::vector<int> fontSizes = {
		8, 10, 12, 14,
		20, 22, 24, 26,
		30, 32, 34, 36,
		40, 42, 44, 46,
		60, 64, 68, 72
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
		mFontData.emplace(fontSizes[size], font);
	}
	return true;
}

void Font::Unload()
{
	for (auto& font : mFontData)
	{
		TTF_CloseFont(font.second);
	}
}

Texture* Font::RenderText(const std::string& textMessage, const Vector3& color, int pointSize, bool blend)
{
	Texture* texture = nullptr;

	// Convert to SDL_Color
	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(color.x * 255);
	sdlColor.g = static_cast<Uint8>(color.y * 255);
	sdlColor.b = static_cast<Uint8>(color.z * 255);
	sdlColor.a = 255;

	// Find the font data for this point size
	auto iter = mFontData.find(pointSize);
	
	if (iter != mFontData.end())
	{
		TTF_Font* font = iter->second;
		
		// Draw this to a surface (blended for alpha
		SDL_Surface* messageSurf = blend ? TTF_RenderUTF8_Blended(font, textMessage.c_str(), sdlColor) : TTF_RenderText_Solid(font, textMessage.c_str(), sdlColor);

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