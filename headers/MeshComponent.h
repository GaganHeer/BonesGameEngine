#pragma once
#include "Component.h"
#include <cstddef>

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner, bool mIsSkeletal = false);
	~MeshComponent();
	virtual void Draw(class Shader* shader);

	virtual void SetMesh(class Mesh* newMesh) { 
		mesh = newMesh; 
	}

	void SetTextureIndex(size_t index) { 
		textureIndex = index; 
	}

	void SetVisible(bool _visible) { visible = _visible; }
	bool GetVisible() const { return visible; }

	bool GetIsSkeletal() const { return isSkeletal; }
protected:
	class Mesh* mesh;
	size_t textureIndex;
	bool visible;
	bool isSkeletal;
};
