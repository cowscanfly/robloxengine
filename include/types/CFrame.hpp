#pragma once


#include "types/Vector3.hpp"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Engine {

class CFrame {
private:
	static glm::vec3 toGlm(const Vector3& v);
	static Vector3 toCustom(const glm::vec3& v);
public:
	glm::mat4 matrix;

	CFrame();
	CFrame(const glm::mat4& mat);
	CFrame(float x, float y, float z);
	CFrame(const Vector3& pos);
	CFrame(const Vector3& pos, const Vector3& lookAtTarget);
	CFrame(float x, float y, float z, float R00, float R01, float R02,
					  float R10, float R11, float R12,
					  float R20, float R21, float R22);

	static CFrame Angles(float rx, float ry, float rz);
	static CFrame lookAt(const Vector3& eye, const Vector3& target, const Vector3& up = Vector3(0.0f, 1.0f, 0.0f));

	CFrame Rotation() const;
	Vector3 Position() const;
	Vector3 RightVector() const;
	Vector3 UpVector() const;
	Vector3 LookVector() const;

	CFrame operator*(const CFrame& other) const;
	Vector3 operator*(const Vector3& vector) const;
	CFrame operator+(const Vector3& translation) const;
	CFrame operator-(const Vector3& translation) const;

	CFrame Inverse() const;
	CFrame ToWorldSpace(const CFrame& localCF) const;
	CFrame ToObjectSpace(const CFrame& worldCF) const;
};
} // namespace Engine
