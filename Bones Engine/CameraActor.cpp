#include "CameraActor.h"
//#include "MoveComponent.h"
#include "SDL_scancode.h"
#include "Renderer.h"
#include "Game.h"

CameraActor::CameraActor(Game* game)
	:Actor(game) {
	//moveComp = new MoveComponent(this);
}

void CameraActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// Compute new camera from this actor
	Eigen::Vector3f cameraPos = GetPosition();
	Eigen::Vector3f target = GetPosition() + GetForward() * 100.0f;
	Eigen::Vector3f up = Eigen::Vector3f::UnitZ();

	Eigen::Matrix4f view = Math::CreateLookAt4f(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
}

void CameraActor::ActorInput(const uint8_t* keys) {
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;

	if (keys[SDL_SCANCODE_W]) {
		forwardSpeed += 300.0f;
	}
	if (keys[SDL_SCANCODE_S]) {
		forwardSpeed -= 300.0f;
	}
	if (keys[SDL_SCANCODE_A]) {
		angularSpeed -= (2 * Math::Pi);
	}
	if (keys[SDL_SCANCODE_D]) {
		forwardSpeed += (2 * Math::Pi);
	}

	//moveComp->SetForwardSpeed(forwardSpeed);
	//moveComp->SetAngularSpeed(angularSpeed);
}