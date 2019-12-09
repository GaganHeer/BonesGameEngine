#include "Actor.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* gameInst)
	:state(State::Active),
	position(Vector3::Zero),
	rotation(Quaternion::Identity),
	scale(1.0f),
	game(gameInst),
	v_scale(scale, scale, scale),
	recomputeWorldTransform(true)
{
	game->AddActor(this);
}

Actor::~Actor()
{
	game->RemoveActor(this);
	
	while (!components.empty())
	{
		delete components.back();
	}
}

void Actor::Update(float deltaTime)
{
	if (state == State::Active)
	{
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		ComputeWorldTransform();
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : components)
	{
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::ProcessInput(InputState keyState)
{
	if (state == State::Active) {
		for (auto comp : components) {
			comp->ProcessInput(keyState);
		}
		ActorInput(keyState);
	}
}

void Actor::ActorInput(InputState keyState)
{
}

void Actor::RotateToNewForward(const Vector3& forward)
{
	// Figure out difference between original (unit x) and new
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);
	// Facing down X
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// Facing down -X
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// Rotate about axis from cross product
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

void Actor::ComputeWorldTransform()
{
	if (recomputeWorldTransform)
	{
		recomputeWorldTransform = false;
		// Scale, then rotate, then translate
		worldTransform = Matrix4::CreateScale(v_scale);
		worldTransform *= Matrix4::CreateFromQuaternion(rotation);
		worldTransform *= Matrix4::CreateTranslation(position);

		// tell components world transform updated
		for (auto comp : components)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

void Actor::AddComponent(Component* component)
{
	int myOrder = component->GetUpdateOrder();
	vector<Component*>::iterator itr;
	for (itr = components.begin(); itr < components.end(); itr++)
	{
		if (myOrder < (*itr)->GetUpdateOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	components.insert(itr, component);
}

void Actor::RemoveComponent(Component* component)
{
	vector<Component*>::iterator itr;
	for (itr = components.begin(); itr < components.end(); itr++)
	{
		if ((*itr) == component)
		{
			components.erase(itr);
			break;
		}
	}
}
