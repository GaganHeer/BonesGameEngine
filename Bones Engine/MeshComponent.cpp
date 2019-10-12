#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "Vertex.h"

MeshComponent::MeshComponent(Actor* owner)
	:Component(owner),
	mesh(nullptr),
	textureIndex(0)
{
	owner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	owner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mesh)
	{
		// Set the world transform
		shader->SetMatrixUniform("uWorldTransform", owner->GetWorldTransform());
		// Set specular power
		shader->SetFloatUniform("uSpecPower", mesh->GetSpecPower());
		// Set the active texture
		Texture* t = mesh->GetTexture(textureIndex);
		if (t) {
			t->SetActive();
		}
		// Set the mesh's vertex array as active
		Vertex* va = mesh->GetVertex();
		va->SetActive();
		// Draw
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}