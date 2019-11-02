#pragma once
#include "Math.h"

class ShadowInfo
{
public:
	ShadowInfo(const Matrix4& proj) :
		projection(proj) {

	}

	inline Matrix4 GetProjection() {
		return projection;
	}
private:
	Matrix4 projection;
};

