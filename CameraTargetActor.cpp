#include "CameraTargetActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "TopDownCamera.h"
#include "MoveComponent.h"

CameraTargetActor::CameraTargetActor(Game* game) :
	Actor(game) {

	moveComponent = new MoveComponent(this);

	meshComponent = new MeshComponent(this);
	meshComponent->SetMesh(game->GetRenderer()->GetMesh("Assets/Cube.obj"));
	SetPosition(Vector3(0.0f, 0.0f, -50.0f));
	SetScale(50.f);

	cameraComponent = new TopDownCamera(this);
	cameraComponent->SnapToIdeal();
}

void CameraTargetActor::ActorInput(InputState keyState) {
	float verticalMove = 0.0f;
	float horizontalMove = 0.0f;

	if (keyState.Keyboard.GetKeyState(SDL_SCANCODE_W) == ButtonState::Pressed)
	{
		verticalMove = 100.0f;
	}
	if (keyState.Keyboard.GetKeyState(SDL_SCANCODE_S) == ButtonState::Pressed)
	{
		verticalMove = -100.0f;
	}
	if (keyState.Keyboard.GetKeyState(SDL_SCANCODE_A) == ButtonState::Pressed)
	{
		horizontalMove = 100.0f;
	}
	if (keyState.Keyboard.GetKeyState(SDL_SCANCODE_D) == ButtonState::Pressed)
	{
		horizontalMove = -100.0f;
	}

	moveComponent->SetHorizontalMove(horizontalMove);
	moveComponent->SetVerticalMove(verticalMove);
}

void CameraTargetActor::SetVisible(bool visible) {
	meshComponent->SetVisible(visible);
}