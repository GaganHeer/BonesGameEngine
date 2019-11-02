#pragma once
#include <vector>

class GBuffer
{
public:
	enum Type {
		Diffuse = 0,
		Normal,
		WorldPos,
		NUM_GBUFFER_TEXTURES
	};
	GBuffer();
	~GBuffer();

	// Create/destroy the G-buffer
	bool Create(int width, int height);
	void Destroy();

	// Get the texture for a specific type of data
	class Texture* GetTexture(Type type);
	// Get the framebuffer object ID
	unsigned int GetBufferID() const { return bufferID; }
	// Setup all the G-buffer textures for sampling
	void SetTexturesActive();
private:
	// Textures associated with G-buffer
	std::vector<class Texture*> textures;
	// Frame buffer object ID
	unsigned int bufferID;
};

