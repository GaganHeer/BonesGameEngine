#pragma once
#include <string>
#include <GL/glew.h>
#include <SOIL.h>
#include <SDL.h>

class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();
	void SetActive();
	int GetWidth() const { return texWidth; }
	int GetHeight() const { return texHeight; }

private:
	// OpenGL ID
	unsigned int textureID;
	int texWidth;
	int texHeight;
};
