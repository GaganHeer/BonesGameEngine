#include "Actor.h"
#include "Component.h"
#include "Game.h"
#include <algorithm>

Actor::Actor(Game* game) :
	state(Active),
	position(Eigen::Vector2f::Zero()),
	scale(1.0f),
	rotation(0.0f),
	game(game) {

	this->game->AddActor(this);
}

Actor::~Actor() {
	game->RemoveActor(this);

	while (!components.empty()) {
		delete components.back();
	}
}

void Actor::Update(float deltaTime) {
	if (state == Active) {
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime) {
	for (auto comp : components) {
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime) {
	
}

void Actor::AddComponent(Component* component) {
	int myOrder = component->GetUpdateOrder();
	auto iter = components.begin();

	for (; iter != components.end(); ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) {
			break;
		}
	}

	components.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
	auto iter = std::find(components.begin(), components.end(), component);
	if (iter != components.end()) {
		components.erase(iter);
	}
}