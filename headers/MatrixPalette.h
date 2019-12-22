#pragma once
#include "MathT.h"

const size_t MAX_SKELETON_BONES = 96;

struct MatrixPalette {
	Matrix4 entry[MAX_SKELETON_BONES];
};