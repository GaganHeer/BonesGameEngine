#include "CameraTargetActor.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "TopDownCamera.h"
#include "MoveComponent.h"

CameraTargetActor::CameraTargetActor(Game* game) :
	Actor(game)
	//,moving(false) 
{

	moveComponent = new MoveComponent(this);

	skeletalMeshComponent = new SkeletalMeshComponent(this);

	skeletalMeshComponent->SetMesh(game->GetRenderer()->GetMesh("Assets/skellyRunForward.gpmesh"));
	skeletalMeshComponent->SetSkeleton(game->GetSkeleton("Assets/skellyRunForward.gpskel"));
	SetPosition(Vector3(0.0f, 0.0f, -100.0f));
	SetScale(25.0f);

	cameraComponent = new TopDownCamera(this);
	cameraComponent->SnapToIdeal();
}

void CameraTargetActor::ActorInput(InputState keyState) {
	float verticalMove = 0.0f;
	float horizontalMove = 0.0f;
	if (keyState.Keyboard.GetKeyState(SDL_SCANCODE_W) == ButtonState::Pressed)
	{
		verticalMove = 100.0f;
		skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/skellyIdleForward.gpanim"), 1.25f);
	}
	if (keyState.Keyboard.GetKeyState(SDL_SCANCODE_S) == ButtonState::Pressed)
	{
		verticalMove = -100.0f;
		skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/skellyIdleBackward.gpanim"), 1.25f);
	}
	if (keyState.Keyboard.GetKeyState(SDL_SCANCODE_A) == ButtonState::Pressed)
	{
		horizontalMove = 100.0f;
		skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/skellyIdleLeft.gpanim"), 1.25f);
	}
	if (keyState.Keyboard.GetKeyState(SDL_SCANCODE_D) == ButtonState::Pressed)
	{
		horizontalMove = -100.0f;
		skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/skellyIdleRight.gpanim"), 1.25f);
	}

	moveComponent->SetHorizontalMove(horizontalMove);
	moveComponent->SetVerticalMove(verticalMove);
}

void CameraTargetActor::SetVisible(bool visible) {
	meshComponent->SetVisible(visible);
}