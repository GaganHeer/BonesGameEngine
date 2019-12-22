#pragma once
#include "BoneTransform.h"
#include <vector>
#include <string>

class Animation
{
public: 
	bool Load(const std::string& fileName);
	
	size_t GetNumBones() const { return numBones; }
	size_t GetNumFrames() const { return numFrames; }
	float GetDuration() const { return duration; }
	float GetFrameDuration() const { return frameDuration; }
	
	// Fills the provided vector with the global (current) pose matrices for each
	// bone at the specified time in the animation. It is expected that the time
	// is >= 0.0f and <= mDuration
	void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const class Skeleton* inSkeleton, float inTime) const;
private:
	// Number of bones for the animation
	size_t numBones;
	// Number of frames in the animation
	size_t numFrames;
	// Duration of the animation in seconds
	float duration;
	// Duration of each frame in animation
	float frameDuration;
	// Transform information for each frame on the track
	// Each index in the outer vector is a bone, inner vector is a frame
	std::vector<std::vector<BoneTransform>> tracks;
};