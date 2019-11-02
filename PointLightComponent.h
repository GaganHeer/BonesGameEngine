#pragma once
#include "Math.h"
#include "Component.h"

class PointLightComponent : public Component
{
public:
	PointLightComponent(class Actor* owner);
	~PointLightComponent();

	void Draw(class Shader* shader, class Mesh* mesh);

	Vector3 diffuseColor;
	float innerRadius;
	float outerRadius;
};

