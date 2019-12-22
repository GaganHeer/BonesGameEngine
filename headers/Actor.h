#pragma once
#include <vector>
#include "MathT.h"
#include <cstdint>
#include "Game.h"
#include "InputSystem.h"
#include "Component.h"

class Actor
{
public:
	enum class State
	{
		Active,
		Paused,
		Dead
	};

	Actor(Game* game);
	virtual ~Actor();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	void ProcessInput(const InputState keyState);
	virtual void ActorInput(InputState keyState);

	void ComputeWorldTransform();
	void RotateToNewForward(const Vector3& forward);
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	// Getters
	const Vector3 GetPosition() const { 
		return position; 
	}

	float GetScale() const { 
		return scale; 
	}

	Vector3 GetScaleV() const { 
		return v_scale; 
	}

	Vector3 GetForward() const { 
		return Vector3::Transform(Vector3::UnitX, rotation); 
	}

	Vector3 GetRight() const {
		return Vector3::Transform(Vector3::UnitY, rotation);
	}

	State GetState() const { 
		return state; 
	}

	const Quaternion& GetRotation() const {
		return rotation;
	}

	const Matrix4& GetWorldTransform() const { 
		return worldTransform; 
	}

	Game* GetGame() { 
		return game; 
	}

	//Setters
	void SetPosition(const Vector3& newPosition) { 
		position = newPosition; recomputeWorldTransform = true; 
	}

	void SetScale(float newScale) {
		scale = newScale;
		SetScaleV(Vector3(newScale, newScale, newScale));
	}

	void SetScaleV(Vector3& newScale) { 
		v_scale = newScale;
		recomputeWorldTransform = true; 
	}

	void SetRotation(const Quaternion& newRotation) { 
		rotation = newRotation;  recomputeWorldTransform = true; 
	}
	
	void SetState(State newState) { 
		state = newState; 
	}

private:
	// Actor's state
	State state;

	// Transform
	Matrix4 worldTransform;
	Vector3 position;
	Quaternion rotation;
	float scale;
	Vector3 v_scale;
	bool recomputeWorldTransform;

	std::vector<class Component*> components;
	Game* game;
};
