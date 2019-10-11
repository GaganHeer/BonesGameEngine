#pragma once
#include <iostream>
#include <vector>
#include <Eigen/Dense>

class Actor
{
public:
	enum State {
		Active,
		Paused,
		Dead
	};

	Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	void ProcessInput(const uint8_t* keyState);
	virtual void ActorInput(const uint8_t* keyState);

	const Eigen::Vector2f& GetPosition() const {
		return this->position;
	}

	void SetPosition(const Eigen::Vector2f& pos) {
		position = pos;
	}

	float GetScale() const {
		return this->scale;
	}

	void SetScale(float scale) {
		this->scale = scale;
	}

	float GetRotation() const {
		return this->rotation;
	}

	void SetRotation(float rot) {
		rotation = rot;
	}

	State GetState() const {
		return this->state;
	}

	void SetState(State state) {
		this->state = state;
	}

	class Game* GetGame() {
		return this->game;
	}

	void ComputeWorldTransform();
	const Eigen::Matrix4f& GetWorldTransform() const { return worldTransform; }

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	State state;

	Eigen::Matrix4f worldTransform;
	Eigen::Vector2f position;
	float scale;
	float rotation;
	bool recomputeWorldTransform;

	std::vector<class Component*> components;
	class Game* game;
};

