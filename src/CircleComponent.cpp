#include "CircleComponent.h"

CircleComponent::CircleComponent(class Actor* newOwner)
	:Component(newOwner),
	radius(0.0f)
{
	
}

const Vector3& CircleComponent::GetCenter() const
{
	return owner->GetPosition();
}

float CircleComponent::GetRadius() const
{
	return owner->GetScale() * radius;
}

bool Intersect(const CircleComponent& a, const CircleComponent& b)
{
	// Calculate distance squared
	Vector3 diff = a.GetCenter() - b.GetCenter();
	float distSq = diff.LengthSq();

	// Calculate sum of radii squared
	float radiiSq = a.GetRadius() + b.GetRadius();
	radiiSq *= radiiSq;

	return distSq <= radiiSq;
}
