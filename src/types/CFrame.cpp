#include "types/CFrame.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "types/Vector3.hpp"

namespace Engine {

glm::vec3 CFrame::toGlm(const Vector3& v) {
	return glm::vec3(v.x, v.y, v.z);
}

Vector3 CFrame::toCustom(const glm::vec3& v) {
	return Vector3(v.x, v.y, v.z);
}

CFrame::CFrame() : matrix(glm::mat4(1.0f)) {}

CFrame::CFrame(const glm::mat4& mat) : matrix(mat) {}

CFrame::CFrame(float x, float y, float z) {
	matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

CFrame::CFrame(const Vector3& pos) {
	matrix = glm::translate(glm::mat4(1.0f), toGlm(pos));
}

CFrame::CFrame(const Vector3& pos, const Vector3& lookAtTarget) {
	*this = CFrame::lookAt(pos, lookAtTarget);
}

CFrame::CFrame(float x, float y, float z, float R00, float R01, float R02,
                  float R10, float R11, float R12,
                  float R20, float R21, float R22)
{
    matrix = glm::mat4(
        glm::vec4(R00, R01, R02, 0.0f), // Column 0
        glm::vec4(R10, R11, R12, 0.0f), // Column 1
        glm::vec4(R20, R21, R22, 0.0f), // Column 2
        glm::vec4(x,   y,   z,   1.0f)  // Column 3
    );
}

CFrame CFrame::Angles(float rx, float ry, float rz) {
	glm::mat4 rot = glm::mat4(1.0f);
	rot = glm::rotate(rot, rz, glm::vec3(0, 0, 1));
	rot = glm::rotate(rot, ry, glm::vec3(0, 1, 0));
	rot = glm::rotate(rot, rx, glm::vec3(1, 0, 0));
	return CFrame(rot);
}

CFrame CFrame::lookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
	glm::vec3 gEye = toGlm(eye);
	glm::vec3 gTarget = toGlm(target);
	glm::vec3 gUp = toGlm(up);

	glm::vec3 zAxis = glm::normalize(gEye - gTarget); 
	glm::vec3 xAxis = glm::normalize(glm::cross(gUp, zAxis));
	glm::vec3 yAxis = glm::cross(zAxis, xAxis);

	glm::mat4 mat(1.0f);
	mat[0] = glm::vec4(xAxis, 0.0f);
	mat[1] = glm::vec4(yAxis, 0.0f);
	mat[2] = glm::vec4(zAxis, 0.0f);
	mat[3] = glm::vec4(gEye, 1.0f);
	
	return CFrame(mat);
}

CFrame CFrame::Rotation() const {
	glm::mat4 rotMat = this->matrix;
	rotMat[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return CFrame(rotMat);
}

Vector3 CFrame::Position() const {
	return toCustom(glm::vec3(matrix[3]));
}

Vector3 CFrame::RightVector() const {
	return toCustom(glm::vec3(matrix[0]));
}

Vector3 CFrame::UpVector() const {
	return toCustom(glm::vec3(matrix[1]));
}

Vector3 CFrame::LookVector() const {
	return toCustom(-glm::vec3(matrix[2]));
}

CFrame CFrame::operator*(const CFrame& other) const {
	return CFrame(this->matrix * other.matrix);
}

Vector3 CFrame::operator*(const Vector3& vector) const {
	glm::vec4 localVec = glm::vec4(toGlm(vector), 1.0f);
	return toCustom(glm::vec3(this->matrix * localVec));
}

CFrame CFrame::operator+(const Vector3& translation) const {
	glm::mat4 newMat = this->matrix;
	newMat[3] += glm::vec4(toGlm(translation), 0.0f);
	return CFrame(newMat);
}

CFrame CFrame::operator-(const Vector3& translation) const {
	glm::mat4 newMat = this->matrix;
	newMat[3] -= glm::vec4(toGlm(translation), 0.0f);
	return CFrame(newMat);
}

CFrame CFrame::Inverse() const {
	return CFrame(glm::inverse(this->matrix));
}

CFrame CFrame::ToWorldSpace(const CFrame& localCF) const {
	return (*this) * localCF;
}

CFrame CFrame::ToObjectSpace(const CFrame& worldCF) const {
	return this->Inverse() * worldCF;
}

} // namespace Engine
