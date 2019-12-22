#include "PointLightComponent.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Actor.h"

PointLightComponent::PointLightComponent(Actor* owner) :
	Component(owner) {

	owner->GetGame()->GetRenderer()->AddPointLight(this);
}

PointLightComponent::~PointLightComponent() {
	owner->GetGame()->GetRenderer()->RemovePointLight(this);
}

void PointLightComponent::Draw(Shader* shader, Mesh* mesh) {
	// We assume, coming into this function, that the shader is active
	// and the sphere mesh is active

	// World transform is scaled to the outer radius (divided by the mesh radius)
	// and positioned to the world position
	Matrix4 scale = Matrix4::CreateScale(owner->GetScale() *
		outerRadius / mesh->GetRadius());
	Matrix4 trans = Matrix4::CreateTranslation(owner->GetPosition());
	Matrix4 worldTransform = scale * trans;
	shader->SetMatrixUniform("uWorldTransform", worldTransform);
	// Set point light shader constants
	shader->SetVectorUniform("uPointLight.mWorldPos", owner->GetPosition());
	shader->SetVectorUniform("uPointLight.mDiffuseColor", diffuseColor);
	shader->SetFloatUniform("uPointLight.mInnerRadius", innerRadius);
	shader->SetFloatUniform("uPointLight.mOuterRadius", outerRadius);

	// Draw the sphere
	glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetNumIndices(),
		GL_UNSIGNED_INT, nullptr);
}