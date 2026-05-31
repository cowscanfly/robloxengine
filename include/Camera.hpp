#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), 
	       glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
	       float yaw = -90.0f, 
	       float pitch = 0.0f);
	~Camera();

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(float aspectRatio) const;

	void ProcessKeyboard(glm::vec3 directionDelta, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

	glm::vec3 GetPosition() const;
	glm::vec3 GetForwardVector() const;
	glm::vec3 GetRightVector() const;
	glm::vec3 GetUpVector() const;
	float GetFOV() const;

private:
	glm::vec3 m_Position;
	glm::vec3 m_Forward;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	float m_Yaw;
	float m_Pitch;

	float m_MovementSpeed;
	float m_MouseSensitivity;
	float m_FOV;

	void UpdateCameraVectors();
};
} // namespace Engine
