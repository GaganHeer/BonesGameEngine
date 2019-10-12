#pragma once
#include "Component.h"
#include <cstddef>
class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();

	virtual void Draw(class Shader* shader);
	virtual void SetMesh(class Mesh* mesh) { this->mesh = mesh; }
	void SetTextureIndex(size_t index) { textureIndex = index; }
protected:
	class Mesh* mesh;
	size_t textureIndex;
};

