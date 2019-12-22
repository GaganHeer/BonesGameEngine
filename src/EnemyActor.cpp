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
	enemyMoveComponent = new EnemyMoveComponent(this);
	Vector3 pos = GetPosition();
}

void EnemyActor::ActorInput(InputState keyState) {
	bool move = false;
	enemyMoveComponent->SetMoveable(moveable);

	if (enemyMoveComponent->movingLeft) {
		skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/Animations/knightRunBackward.json"), 1.25f);
	}
	else if (enemyMoveComponent->movingUp) {
		skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/Animations/knightRunRight.json"), 1.25f);
	}
	else if (enemyMoveComponent->movingDown) {
		skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/Animations/knightRunLeft.json"), 1.25f);
	}
	else if (enemyMoveComponent->movingRight) {
		skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/Animations/knightRunForward.json"), 1.25f);
	}

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
	skeletalMeshComponent->SetMesh(game->GetRenderer()->GetMesh("Assets/Animations/knightMesh.obj"));
	skeletalMeshComponent->SetSkeleton(game->GetSkeleton("Assets/Animations/knightSkel.json"));
	skeletalMeshComponent->PlayAnimation(GetGame()->GetAnimation("Assets/Animations/knightIdleBackward.json"), 1.25f);
	SetScale(100.0f);
}