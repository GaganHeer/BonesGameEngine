#include "Texture.h"

Texture::Texture()
	:textureID(0), 
	texWidth(0),
	texHeight(0) {

}

Texture::~Texture() {
}

bool Texture::Load(const std::string& fileName) {
	int channels = 0;

	unsigned char* image = SOIL_load_image(fileName.c_str(), &texWidth, &texHeight, 
											&channels, SOIL_LOAD_AUTO);

	if (image == nullptr) {
		SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	int format = GL_RGB;
	if (channels == 4) {
		format = GL_RGBA;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, 
					GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload() {
	glDeleteTextures(1, &textureID);
}

void Texture::SetActive() {
	glBindTexture(GL_TEXTURE_2D, textureID);
}
