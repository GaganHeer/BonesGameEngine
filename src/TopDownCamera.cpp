#include "TopDownCamera.h"
#include "Actor.h"

TopDownCamera::TopDownCamera(Actor* owner) :
	CameraComponent(owner),
	horzDist(150.0f),
	vertDist(500.0f),
	targetDist(150.0f),
	springConstant(64.0f) {
}

void TopDownCamera::Update(float deltaTime) {
	CameraComponent::Update(deltaTime);

	Vector3 idealPos = ComputeCameraPos();
	// Target is target dist in front of owning actor
	Vector3 target = owner->GetPosition() + owner->GetForward() * targetDist;
	// Use actual position here, not ideal
	Matrix4 view = Matrix4::CreateLookAt(idealPos, target, Vector3::UnitZ);
	SetViewMatrix(view);
}

void TopDownCamera::SnapToIdeal()
{
	// Set actual position to ideal
	actualPos = ComputeCameraPos();
	// Zero velocity
	velocity = Vector3::Zero;
	// Compute target and view
	Vector3 target = owner->GetPosition() +
		owner->GetForward() * targetDist;
	// Use actual position here, not ideal
	Matrix4 view = Matrix4::CreateLookAt(actualPos, target,
		Vector3::UnitZ);
	SetViewMatrix(view);
}

Vector3 TopDownCamera::ComputeCameraPos() const
{
	// Set camera position behind and above owner
	Vector3 cameraPos = owner->GetPosition();
	cameraPos -= owner->GetForward() * horzDist;
	cameraPos += Vector3::UnitZ * vertDist;
	return cameraPos;
}