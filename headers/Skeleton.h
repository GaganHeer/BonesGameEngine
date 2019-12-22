#pragma once
#include "BoneTransform.h"
#include <string>
#include <vector>

class Skeleton 
{
public: 
	// Definition for each bone in the skeleton
	struct Bone 
	{
		BoneTransform localBindPose;
		std::string name;
		int parent;
	};
	
	//Load a file
	bool Load(const std::string& fileName);
	
	//Getter functions
	size_t GetNumBones() const { return bones.size(); }
	const Bone& GetBone(size_t idx) const { return bones[idx]; }
	const std::vector<Bone>& GetBones() const { return bones; }
	const std::vector<Matrix4>& GetGlobalInvBindPoses() const { return globalInvBindPoses; }
protected:
	//Computes the global inverse bind pose for each bone
	//Called when loading the skeleton
	void ComputeGlobalInvBindPose();
private:
	//The bones in the skeleton
	std::vector<Bone> bones;
	//The global inverse bind poses for each bone
	std::vector<Matrix4> globalInvBindPoses;
};