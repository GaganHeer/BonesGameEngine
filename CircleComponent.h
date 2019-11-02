#pragma once
#include "Component.h"
#include "Math.h"
#include "Actor.h"

class CircleComponent : public Component
{
public:
	CircleComponent(Actor* owner);
	
	void SetRadius(float newRadius) { radius = newRadius; }
	float GetRadius() const;
	
	const Vector3& GetCenter() const;
private:
	float radius;
};

bool Intersect(const CircleComponent& a, const CircleComponent& b);
