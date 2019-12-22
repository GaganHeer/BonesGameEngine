#include "Skeleton.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL.h>
#include "MatrixPalette.h"

bool Skeleton::Load(const std::string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Skeleton %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Skeleton %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// Check the metadata
	if (ver != 1)
	{
		SDL_Log("Skeleton %s unknown format", fileName.c_str());
		return false;
	}

	const rapidjson::Value& bonecount = doc["bonecount"];
	if (!bonecount.IsUint())
	{
		SDL_Log("Skeleton %s doesn't have a bone count.", fileName.c_str());
		return false;
	}

	size_t count = bonecount.GetUint();

	if (count > MAX_SKELETON_BONES)
	{
		SDL_Log("Skeleton %s exceeds maximum bone count.", fileName.c_str());
		return false;
	}

	bones.reserve(count);

	const rapidjson::Value& mBones = doc["bones"];
	if (!mBones.IsArray())
	{
		SDL_Log("Skeleton %s doesn't have a bone array?", fileName.c_str());
		return false;
	}

	if (mBones.Size() != count)
	{
		SDL_Log("Skeleton %s has a mismatch between the bone count and number of bones", fileName.c_str());
		return false;
	}

	Bone temp;

	for (rapidjson::SizeType i = 0; i < count; i++)
	{
		if (!mBones[i].IsObject())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		const rapidjson::Value& name = mBones[i]["name"];
		temp.name = name.GetString();

		const rapidjson::Value& parent = mBones[i]["parent"];
		temp.parent = parent.GetInt();

		const rapidjson::Value& bindpose = mBones[i]["bindpose"];
		if (!bindpose.IsObject())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		const rapidjson::Value& rot = bindpose["rot"];
		const rapidjson::Value& trans = bindpose["trans"];

		if (!rot.IsArray() || !trans.IsArray())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		temp.localBindPose.rotation.x = rot[0].GetDouble();
		temp.localBindPose.rotation.y = rot[1].GetDouble();
		temp.localBindPose.rotation.z = rot[2].GetDouble();
		temp.localBindPose.rotation.w = rot[3].GetDouble();

		temp.localBindPose.translation.x = trans[0].GetDouble();
		temp.localBindPose.translation.y = trans[1].GetDouble();
		temp.localBindPose.translation.z = trans[2].GetDouble();

		bones.emplace_back(temp);
	}

	// Now that we have the bones
	ComputeGlobalInvBindPose();

	return true;
}
void Skeleton::ComputeGlobalInvBindPose()
{
	// Resize to number of bones, which automatically fills identity
	globalInvBindPoses.resize(GetNumBones());
	
	// Step 1: Compute global bind pose for each bone
	// The global bind pose for root is just the local bind bose
	globalInvBindPoses[0] = bones[0].localBindPose.ToMatrix();
	
	// Each remaining bone's global bind pose is its local pose
	// multiplied by the parent's global bind pose
	for (size_t i = 1; i < globalInvBindPoses.size(); i++) 
	{
		Matrix4	localMat = bones[i].localBindPose.ToMatrix();
		globalInvBindPoses[i] = localMat * globalInvBindPoses[bones[i].parent];
	}
	
	//Step 2: Invert each matrix
	for (size_t i = 0; i < globalInvBindPoses.size(); i++)
	{
		globalInvBindPoses[i].Invert();
	}
}