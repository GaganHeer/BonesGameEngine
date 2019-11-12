#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"


SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner) 
	:MeshComponent(owner, true),
	skeleton(nullptr)
	{
	}

void SkeletalMeshComponent::Draw(Shader* shader) 
{
	if (mesh)
	{
		// Set up the world transform
		shader->SetMatrixUniform("uWorldTransform", owner->GetWorldTransform());
		// Set the matrix palette
		shader->SetMatrixUniforms("uMatrixPalette", &palette.entry[0], MAX_SKELETON_BONES);
		// Set specular power
		shader->SetFloatUniform("uSpecPower", mesh->GetSpecPower());
		// Set the active texture
		Texture* t = mesh->GetTexture(textureIndex);
		if (t) 
		{
			t->SetActive();
		}
		// Set the mesh's vertex array as active
		VertexArray* va = mesh->GetVertexArray();
		if (va != nullptr)
		{
			va->SetActive();
			glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
		}
	}
}
		

void SkeletalMeshComponent::Update(float deltaTime)
{
	if (animation && skeleton)
	{
		animTime += deltaTime * animPlayRate;
		// Wrap around anim time if past duration
		while (animTime > animation->GetDuration())
		{
			animTime -= animation->GetDuration();
		}

		// Recompute matrix palette
		ComputeMatrixPalette();
	}
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim, float playRate)
{
	animation = anim;
	animTime = 0.0f;
	animPlayRate = playRate;
	
	if (!animation) { return 0.0f; }
	ComputeMatrixPalette();
	
	return animation->GetDuration();
}

void SkeletalMeshComponent::ComputeMatrixPalette() 
{
	const std::vector<Matrix4>& globalInvBindPoses = skeleton->GetGlobalInvBindPoses();
	std::vector<Matrix4> currentPoses;
	animation->GetGlobalPoseAtTime(currentPoses, skeleton, animTime);
	
	// Setup the palette for each bone
	for (size_t i = 0; i < skeleton->GetNumBones(); i++)
	{
		// Global inverse bind pose matrix times current pose matrix
		palette.entry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}