#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

Camera::~Camera() {}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: m_Position(position)
	, m_WorldUp(up)
	, m_Yaw(yaw)
	, m_Pitch(pitch)
	, m_MovementSpeed(50.0f)       // units per second
	, m_MouseSensitivity(0.035f)    
	, m_FOV(70.0f)                
{
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const {
	return glm::perspective(glm::radians(m_FOV), aspectRatio, 0.1f, 2000.0f);
}

void Camera::ProcessKeyboard(glm::vec3 directionDelta, float deltaTime) {
	float velocity = m_MovementSpeed * deltaTime;
	m_Position += directionDelta * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
	xOffset *= m_MouseSensitivity;
	yOffset *= m_MouseSensitivity;

	m_Yaw   += xOffset;
	m_Pitch += yOffset;

	// Constrain pitch so the player's head can't completely flip upside down
	if (constrainPitch) {
		if (m_Pitch > 89.0f)  m_Pitch = 89.0f;
		if (m_Pitch < -89.0f) m_Pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	
	m_Forward = glm::normalize(front);

	m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));
	m_Up    = glm::normalize(glm::cross(m_Right, m_Forward));
}

glm::vec3 Camera::GetPosition() const      { return m_Position; }
glm::vec3 Camera::GetForwardVector() const   { return m_Forward; }
glm::vec3 Camera::GetRightVector() const     { return m_Right; }
glm::vec3 Camera::GetUpVector() const        { return m_Up; }
float Camera::GetFOV() const                 { return m_FOV; }

} // namespace Engine

