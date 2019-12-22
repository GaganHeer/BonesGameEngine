#pragma once
#include "MeshComponent.h"
#include "MatrixPalette.h"

class SkeletalMeshComponent : public MeshComponent {

public: 
	SkeletalMeshComponent(class Actor* owner);
	// Draw this mesh component
	void Draw(class Shader* shader) override;
	
	void Update(float deltaTime) override;
	
	// Setters
	void SetSkeleton(const class Skeleton* sk) { skeleton = sk; }
	
	// Play an animation
	float PlayAnimation(const class Animation* anim, float playRate = 1.0f);
protected:
	void ComputeMatrixPalette();

	// Matrix Palette
	MatrixPalette palette;
	// Skeleton of object
	const class Skeleton* skeleton;
	// Animation currently playing
	const class Animation* animation;
	// Play rate of animation (1.0 is normal speed)
	float animPlayRate;
	// Current time in the animation
	float animTime;
};