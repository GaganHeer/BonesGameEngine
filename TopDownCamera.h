#pragma once
#include "CameraComponent.h"
#include "InputSystem.h"

class TopDownCamera : public CameraComponent
{
public:
	TopDownCamera(class Actor* owner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	void SetHorzDist(float dist) { horzDist = dist; }
	void SetVertDist(float dist) { vertDist = dist; }
	void SetTargetDist(float dist) { targetDist = dist; }
	void SetSpringConstant(float spring) { springConstant = spring; }
private:
	Vector3 ComputeCameraPos() const;
	Vector3 actualPos;
	Vector3 velocity;
	// Horizontal follow distance
	float horzDist;
	// Vertical follow distance
	float vertDist;
	// Target distance
	float targetDist;
	// Spring constant (higher is more stiff)
	float springConstant;
};

