#pragma once
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "InputSystem.h";

class Actor {
public:
	enum class State {
		Active,
		Paused,
		Dead
	};

	Actor(class Game* game);
	virtual ~Actor();

	//Update
	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	//Process input
	void ProcessInput(InputState keyState);
	virtual void ActorInput(InputState keyState);

	//Getters and Setters
	//Position
	const Eigen::Vector3f& GetPosition() const {
		return this->position;
	}

	void SetPosition(const Eigen::Vector3f& pos) {
		position = pos;
	}

	//Scale
	float GetScale() const {
		return this->scale;
	}

	void SetScale(float scale) {
		this->scale = scale;
	}

	//Rotation
	Eigen::Quaternionf GetRotation() const {
		return this->rotation;
	}

	void SetRotation(Eigen::Quaternionf quaternion) {
		this->rotation = quaternion;
	}

	//State
	State GetState() const {
		return state;
	}

	void SetState(State state) {
		this->state = state;
	}

	//World transform
	void ComputeWorldTransform();
	const Eigen::Matrix4f& GetWorldTransform() {
		return this->worldTransform;
	}

	//Forward
	Eigen::Vector3f GetForward() const {
		return rotation * Eigen::Vector3f::UnitX();
	}

	//Game
	class Game* GetGame() {
		return this->game;
	}

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	State state;

	Eigen::Matrix4f worldTransform;
	Eigen::Vector3f position;
	Eigen::Quaternionf rotation;
	float scale;
	bool recomputeWorldTransform;

	std::vector<class Component*> components;
	class Game* game;
};

