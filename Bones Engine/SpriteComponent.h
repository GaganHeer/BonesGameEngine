#pragma once

#include "Component.h"
#include <SDL.h>
#include <Eigen/Dense>
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Math.h"

class SpriteComponent : public Component {
public:
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(class Shader* shader);
	virtual void SetTexture(class Texture* texture);

	int GetDrawOrder() const { return drawOrder; }
	int GetTexHeight() const { return texHeight; }
	int GetTexWidth() const { return texWidth; }
protected:
	class Texture* texture;
	int drawOrder;
	int texWidth;
	int texHeight;
};
