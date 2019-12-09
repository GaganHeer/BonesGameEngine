#include "EnemyActor.h"
#include "EnemyMoveComponent.h"
#include "SkeletalMeshComponent.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "TopDownCamera.h"

EnemyActor::EnemyActor(Game* game) :
	Actor(game),
	moveable(false)
{
	
	//meshComponent = new MeshComponent(this);
	//meshComponent->SetMesh(game->GetRenderer()->GetMesh("Assets/skellyMesh.json"));
	
	skeletalMeshComponent = new SkeletalMeshComponent(this);
	skeletalMeshComponent->SetMesh(game->GetRenderer()->GetMesh("Assets/knightIdle.gpmesh"));
	skeletalMeshComponent->SetSkeleton(game->GetSkeleton("Assets/knightIdle.gpskel"));
	//skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/knightIdle.gpanim"), 1.25f);
	
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