#pragma once
#include "Component.h"
#include "SDL.h"
class SpriteComponent : public Component
{
public:

	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(class Shader* shader);
	virtual void SetTexture(class Texture* texture);

	int GetDrawOrder() const { 
		return drawOrder; 
	}

	int GetTexHeight() const { 
		return texHeight; 
	}

	int GetTexWidth() const { 
		return texWidth; 
	}

	void SetVisible(bool _visible) { visible = _visible; }
	bool GetVisible() const { return visible; }
protected:
	class Texture* texture;
	int drawOrder;
	int texWidth;
	int texHeight;
	bool visible;
};
