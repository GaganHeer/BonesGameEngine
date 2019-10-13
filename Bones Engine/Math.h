#pragma once
#include <cmath>
#include <memory.h>
#include <limits>
#include <Eigen/Dense>

namespace Math {

	const float Pi = 3.1415926535f;

	static inline float ToRadians(float degrees)
	{
		return degrees * Pi / 180.0f;
	}

	static inline float ToDegrees(float radians)
	{
		return radians * 180.0f / Pi;
	}

	template <typename T>
	T Max(const T& a, const T& b) {
		return (a < b ? b : a);
	}

	template <typename T>
	T Min(const T& a, const T& b) {
		return (a < b ? a : b);
	}

	static float LengthSq(const Eigen::Vector3f& vector) {
		return (vector.x() * vector.x() + vector.y() * vector.y() + vector.z() * vector.z());
	}

	static float Length(const Eigen::Vector3f& vector) {
		return sqrt(LengthSq(vector));
	}

	static Eigen::Vector3f GetTranslation4f(const Eigen::Matrix4f* mat) {
		return Eigen::Vector3f(mat->coeff(3, 0), mat->coeff(3, 1), mat->coeff(3, 2));
	}

	static const float* GetAsFloatPtrV3f(const Eigen::Vector3f& vector) {
		return reinterpret_cast<const float*>(&vector[0]);
	}

	static const float* GetAsFloatPtr4f(const Eigen::Matrix4f* mat) {
		return reinterpret_cast<const float*>(&mat->coeff(0, 0));
	}


	static Eigen::Vector3f GetScale4f(Eigen::Matrix4f* mat) {
		Eigen::Vector3f retVal(sqrt(powf(mat->coeff(0, 0), 2) + powf(mat->coeff(0, 1), 2) + powf(mat->coeff(0, 2), 2)),
			sqrt(powf(mat->coeff(1, 0), 2) + powf(mat->coeff(1, 1), 2) + powf(mat->coeff(1, 2), 2)),
			sqrt(powf(mat->coeff(2, 0), 2) + powf(mat->coeff(2, 1), 2) + powf(mat->coeff(2, 2), 2)));
		return retVal;
	}

	static Eigen::Matrix4f CreateScale4f(float xScale, float yScale, float zScale) {
		Eigen::Matrix4f retVal;
		retVal << xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, zScale, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f;
		return retVal;
	}

	static Eigen::Matrix4f CreateScale4f(const Eigen::Vector3f& scaleVector) {
		return CreateScale4f(scaleVector[0], scaleVector[1], scaleVector[2]);
	}

	static Eigen::Matrix4f CreateScale4f(float scale) {
		return CreateScale4f(scale, scale, scale);
	}

	static Eigen::Matrix4f CreateRotationX4f(float theta) {
		Eigen::Matrix4f retVal;
		retVal << 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cos(theta), sin(theta), 0.0f,
			0.0f, -sin(theta), cos(theta), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f;
		return retVal;
	}

	static Eigen::Matrix4f CreateRotationY4f(float theta) {
		Eigen::Matrix4f retVal;
		retVal << cos(theta), 0.0f, -sin(theta), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sin(theta), 0.0f, cos(theta), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f;
		return retVal;
	}

	static Eigen::Matrix4f CreateRotationZ4f(float theta) {
		Eigen::Matrix4f retVal;
		retVal << cos(theta), sin(theta), 0.0f, 0.0f,
			-sin(theta), cos(theta), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f;
		return retVal;
	}

	static Eigen::Matrix4f CreateTranslation4f(const Eigen::Vector3f& trans) {
		Eigen::Matrix4f retVal;
		retVal << 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			trans[0], trans[1], trans[2], 1.0f;
		return retVal;
	}

	static Eigen::Matrix4f CreateLookAt4f(const Eigen::Vector3f& eye, const Eigen::Vector3f& target, const Eigen::Vector3f& up) {
		Eigen::Vector3f zaxis = (target - eye).normalized();
		Eigen::Vector3f xaxis = (target - eye).normalized();
		Eigen::Vector3f yaxis = (target - eye).normalized();
		Eigen::Vector3f trans(-xaxis.dot(eye),
			-yaxis.dot(eye),
			-zaxis.dot(eye));

		Eigen::Matrix4f retVal;
		retVal << xaxis[0], yaxis[0], zaxis[0], 0.0f,
			xaxis[1], yaxis[1], zaxis[1], 0.0f,
			xaxis[2], yaxis[2], zaxis[2], 0.0f,
			trans[0], trans[1], trans[2], 1.0f;
		return retVal;
	}

	static Eigen::Matrix4f CreateOrtho4f(float width, float height, float near, float far) {
		Eigen::Matrix4f retVal;
		retVal << 2.0f / width, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / height, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (far - near), 0.0f,
			0.0f, 0.0f, near / (near - far), 1.0f;
		return retVal;
	}

	static Eigen::Matrix4f CreatePerspectiveFOV4f(float fovY, float width, float height, float near, float far) {
		float yScale = 1 / tanf(fovY / 2.0f);
		float xScale = yScale * height / width;
		Eigen::Matrix4f retVal;
		retVal << xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, far / (far - near), 1.0f,
			0.0f, 0.0f, -near * far / (far - near), 0.0f;
		return retVal;
	}

	static Eigen::Matrix4f CreateSimpleViewProj4f(float width, float height) {
		Eigen::Matrix4f retVal;
		retVal << 2.0f / width, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / height, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 1.0f;
		return retVal;
	}

	static Eigen::Matrix4f CreateFromQuaternion4f(const Eigen::Quaternionf& q) {
		Eigen::Matrix4f retVal;

		retVal(0, 0) = 1.0f - 2.0f * q.y() * q.y() - 2.0f * q.z() * q.z();
		retVal(0, 1) = 2.0f * q.x() * q.y() + 2.0f * q.w() * q.z();
		retVal(0, 2) = 2.0f * q.x() * q.z() - 2.0f * q.w() * q.y();
		retVal(0, 3) = 0.0f;

		retVal(1, 0) = 2.0f * q.x() * q.y() - 2.0f * q.w() * q.z();
		retVal(1, 1) = 1.0f - 2.0f * q.x() * q.x() - 2.0f * q.z() * q.z();
		retVal(1, 2) = 2.0f * q.y() * q.z() + 2.0f * q.w() * q.x();
		retVal(1, 3) = 0.0f;

		retVal(2, 0) = 2.0f * q.x() * q.z() + 2.0f * q.w() * q.y();
		retVal(2, 1) = 2.0f * q.y() * q.z() - 2.0f * q.w() * q.x();
		retVal(2, 2) = 1.0f - 2.0f * q.x() * q.x() - 2.0f * q.y() * q.y();
		retVal(2, 3) = 0.0f;

		retVal(3, 0) = 0.0f;
		retVal(3, 1) = 0.0f;
		retVal(3, 2) = 0.0f;
		retVal(3, 3) = 1.0f;

		return retVal;
	}

	static Eigen::Quaternionf CreateQuaternionFromAngleAxis(const Eigen::Vector3f& axis, float angle) {
		Eigen::Quaternionf retVal;
		float scalar = sin(angle / 2.0f);
		retVal.x() = axis.x() * scalar;
		retVal.y() = axis.y() * scalar;
		retVal.z() = axis.z() * scalar;
		retVal.w() = cos(angle / 2.0f);
		return retVal;
	}

	static Eigen::Quaternionf CreateQuaternionFromConcatenation(const Eigen::Quaternionf& q, const Eigen::Quaternionf& p) {
		Eigen::Quaternionf retVal;

		Eigen::Vector3f qv(q.x(), q.y(), q.z());
		Eigen::Vector3f pv(p.x(), p.y(), p.z());
		Eigen::Vector3f newVec = p.w() * qv + q.w() * pv + pv.cross(qv);
		retVal.x() = newVec.x();
		retVal.y() = newVec.y();
		retVal.z() = newVec.z();

		retVal.w() = p.w() * q.w() - pv.dot(qv);

		return retVal;
	}
};