#include "Actor.h"
#include "Component.h"
#include "Game.h"
#include <algorithm>
#include "Math.h"

Actor::Actor(Game* game) :
	state(Active),
	position(Eigen::Vector3f::Zero()),
	scale(1.0f),
	rotation(Eigen::Quaternionf::Identity()),
	game(game),
	recomputeWorldTransform(true) {

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
		ComputeWorldTransform();
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
		ComputeWorldTransform();
	}
}

void Actor::UpdateComponents(float deltaTime) {
	for (auto comp : components) {
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime) {
	
}

void Actor::ProcessInput(const uint8_t* keyState)
{
	if (state == Active) {
		for (auto comp : components) {
			comp->ProcessInput(keyState);
		}
		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState)
{
}

void Actor::ComputeWorldTransform() {
	if (recomputeWorldTransform) {
		recomputeWorldTransform = false;
		worldTransform = Math::CreateScale4f(scale);
		worldTransform *= Math::CreateFromQuaternion4f(rotation);
		worldTransform *= Math::CreateTranslation4f(position);

		for (auto comp : components) {
			comp->OnUpdateWorldTransform();
		}
	}
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
