#include "Actor.h"
#include "Component.h"
#include "Game.h"
#include <algorithm>
#include "Math.h"

Actor::Actor(Game* game) :
	state(Active),
	position(Eigen::Vector2f::Zero()),
	scale(1.0f),
	rotation(0.0f),
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

void Actor::ComputeWorldTransform()
{
	if (recomputeWorldTransform)
	{
		recomputeWorldTransform = false;

		//******** 2D *********
		// Scale, then rotate, then translate
		worldTransform = Math::CreateScale4f(scale, scale, 1.0f);
		worldTransform *= Math::CreateRotationZ4f(rotation);

		Eigen::Matrix4f retVal;
		retVal << 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			position[0], position[1], 1.0f, 1.0f;
		worldTransform *= retVal;

		//*****************************************************

		// Inform components world transform updated
		for (auto comp : components) {
			comp->OnUpdateWorldTransform();
		}
	}
}
