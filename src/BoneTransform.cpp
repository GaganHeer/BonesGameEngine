#include "BoneTransform.h"

Matrix4 BoneTransform::ToMatrix() const 
{
	Matrix4 rot = Matrix4::CreateFromQuaternion(rotation);
	Matrix4 trans = Matrix4::CreateTranslation(translation);
	
	return rot * trans;
}

BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
	BoneTransform retVal;
	retVal.rotation = Quaternion::Slerp(a.rotation, b.rotation, f);
	retVal.translation = Vector3::Lerp(a.translation, b.translation, f);
	
	return retVal;
}
	