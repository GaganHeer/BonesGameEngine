#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateOrder) :
	owner(owner),
	updateOrder(updateOrder) {

	this->owner->AddComponent(this);
}

Component::~Component() {
	this->owner->RemoveComponent(this);
}

void Component::Update(float deltaTime) {

}