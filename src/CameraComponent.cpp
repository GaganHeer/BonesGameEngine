#include "CameraComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "Game.h"
#include "AudioEngine.h"

CameraComponent::CameraComponent(Actor* owner, int updateOrder) :
	Component(owner, updateOrder) {

}

void CameraComponent::SetViewMatrix(const Matrix4& view) {
	Game* game = owner->GetGame();
	game->GetRenderer()->SetViewMatrix(view);
}