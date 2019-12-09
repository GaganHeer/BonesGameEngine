#include "EnemyActor.h"
#include "EnemyMoveComponent.h"
#include "SkeletalMeshComponent.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "TopDownCamera.h"

EnemyActor::EnemyActor(Game* game) :
	Actor(game),
	moveable(false),
	game(game)
{
	/*
	skeletalMeshComponent = new SkeletalMeshComponent(this);
	skeletalMeshComponent->SetMesh(game->GetRenderer()->GetMesh("Assets/knightMesh.json"));
	skeletalMeshComponent->SetSkeleton(game->GetSkeleton("Assets/knightSkel.json"));

	SetScale(0.5f);
	*/
	enemyMoveComponent = new EnemyMoveComponent(this);
	Vector3 pos = GetPosition();
}

void EnemyActor::ActorInput(InputState keyState) {
	bool move = false;
	enemyMoveComponent->SetMoveable(moveable);

	if (keyState.Keyboard.GetKeyState(SDL_SCANCODE_W) == ButtonState::Pressed || keyState.Keyboard.GetKeyState(SDL_SCANCODE_S) == ButtonState::Pressed ||
		keyState.Keyboard.GetKeyState(SDL_SCANCODE_A) == ButtonState::Pressed || keyState.Keyboard.GetKeyState(SDL_SCANCODE_D) == ButtonState::Pressed)
	{
		move = true;
	}

	enemyMoveComponent->SetMove(move);
}

void EnemyActor::SetVisible(bool visible) {
	meshComponent->SetVisible(visible);
}

void EnemyActor::SetSkeletalMesh() {
	skeletalMeshComponent = new SkeletalMeshComponent(this);
	skeletalMeshComponent->SetMesh(game->GetRenderer()->GetMesh("Assets/knightMesh.json"));
	skeletalMeshComponent->SetSkeleton(game->GetSkeleton("Assets/knightSkel.json"));
	SetScale(0.5f);
}