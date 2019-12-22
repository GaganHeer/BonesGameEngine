#include "CameraTargetActor.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "TopDownCamera.h"
#include "MoveComponent.h"

CameraTargetActor::CameraTargetActor(Game* game) :
	Actor(game),
	dir(0),
	idle(true)
{

	moveComponent = new MoveComponent(this);

	skeletalMeshComponent = new SkeletalMeshComponent(this);

	skeletalMeshComponent->SetMesh(game->GetRenderer()->GetMesh("Assets/Animations/skellyRunMesh.obj"));
	skeletalMeshComponent->SetSkeleton(game->GetSkeleton("Assets/Animations/skellyRunSkel.json"));
	SetPosition(Vector3(0.0f, 0.0f, -100.0f));
	SetScale(25.0f);

	cameraComponent = new TopDownCamera(this);
	cameraComponent->SnapToIdeal();
}

void CameraTargetActor::ActorInput(InputState keyState) {

	if (idle) {
		if (dir == 0 && !moveComponent->movingUp) {
			skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/Animations/skellyIdleForward.json"), 1.25f);
			idle = false;
		}
		else if (dir == 1 && !moveComponent->movingDown) {
			skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/Animations/skellyIdleBackward.json"), 1.25f);
			idle = false;
		}
		else if (dir == 2 && !moveComponent->movingRight) {
			skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/Animations/skellyIdleLeft.json"), 1.25f);
			idle = false;
		}
		else if (dir == 3 && !moveComponent->movingLeft) {
			skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/Animations/skellyIdleRight.json"), 1.25f);
			idle = false;
		}
	}

	float verticalMove = 0.0f;
	float horizontalMove = 0.0f;
	if (keyState.Keyboard.GetKeyState(SDL_SCANCODE_W) == ButtonState::Pressed)
	{
		verticalMove = 100.0f;
		dir = 0;
		idle = true;
		skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/Animations/skellyRunForward.json"), 1.25f);
	}
	if (keyState.Keyboard.GetKeyState(SDL_SCANCODE_S) == ButtonState::Pressed)
	{
		verticalMove = -100.0f;
		dir = 1;
		idle = true;
		skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/Animations/skellyRunBackward.json"), 1.25f);
	}
	if (keyState.Keyboard.GetKeyState(SDL_SCANCODE_A) == ButtonState::Pressed)
	{
		horizontalMove = 100.0f;
		dir = 2;
		idle = true;
		skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/Animations/skellyRunLeft.json"), 1.25f);
	}
	if (keyState.Keyboard.GetKeyState(SDL_SCANCODE_D) == ButtonState::Pressed)
	{
		horizontalMove = -100.0f;
		dir = 3;
		idle = true;
		skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/Animations/skellyRunRight.json"), 1.25f);
	}

	moveComponent->SetHorizontalMove(horizontalMove);
	moveComponent->SetVerticalMove(verticalMove);
}

void CameraTargetActor::SetVisible(bool visible) {
	meshComponent->SetVisible(visible);
}