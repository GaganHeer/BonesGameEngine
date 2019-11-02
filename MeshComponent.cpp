#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include <assert.h>

MeshComponent::MeshComponent(Actor* newOwner)
	:Component(newOwner),
	mesh(nullptr),
	textureIndex(0),
	visible(true)
{
	owner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent(){
	owner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader){
	if (mesh){
		shader->SetMatrixUniform("uWorldTransform", owner->GetWorldTransform());
		shader->SetFloatUniform("uSpecPower", mesh->GetSpecPower());
		Texture* t = mesh->GetTexture(textureIndex);
		if (t){
			t->SetActive();
		}
		VertexArray* va = mesh->GetVertexArray();
		va->SetActive();
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}