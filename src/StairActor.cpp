#include "StairActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"

StairActor::StairActor(Game* game)
	:Actor(game) {
	//SetScale(10.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Stairs.obj");
	mc->SetMesh(mesh);
}

void StairActor::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	//Quaternion q = Quaternion::Concatenate(GetRotation(), Quaternion::Normalize(Quaternion(Vector3(1.f, 1.f, 1.f), Math::Pi / 4.0f * deltaTime)));
	//SetRotation(q);
	SetScale(4.0f);
}