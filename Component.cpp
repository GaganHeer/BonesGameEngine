#include "Component.h"
#include "Actor.h"

Component::Component(Actor* newOwner, int newUpdateOrder)
	:owner(newOwner)
	,updateOrder(newUpdateOrder)
{
	// Add to actor's vector of components
	owner->AddComponent(this);
}

Component::~Component()
{
	owner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{
}
