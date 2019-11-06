#include <string>

class Texture
{
public:
	Texture();
	~Texture();
	
	bool Load(const std::string& fileName);
	void Unload();
	void CreateFromSurface(struct SDL_Surface* surface);
	void CreateForRendering(int _width, int _height, unsigned int format);
	void CreateForRenderingShadowMap(int _width, int _height, unsigned int format);
	
	void SetActive(int index = 0);
	
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	unsigned int GetTextureID() const { return textureID; }
private:
	unsigned int textureID;
	int width;
	int height;
};
