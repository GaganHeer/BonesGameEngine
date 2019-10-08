#pragma once
#include <cmath>
#include <memory.h>
#include <limits>
#include <Eigen/Dense>

namespace Math {

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

};