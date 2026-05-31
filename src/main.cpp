#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "Instances/Workspace.hpp"
#include "Instances/BasePart.hpp"
#include "roblox_parts.hpp"
#include "types/CFrame.hpp"
#include "types/Color3.hpp"
#include "types/Vector3.hpp"

int main() {
	Engine::Window mainWindow(800, 600, "tung tung aura");
	if (!mainWindow.Initialize()) {
		return -1;
	}

	Engine::Input::Initialize(&mainWindow);

	Engine::Camera camera(glm::vec3(3.0f, 4.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -125.0f, -30.0f);
	Engine::Workspace* workspace = new Engine::Workspace();
	for (int i = 0; i < PART_COUNT; i++) {
		RobloxPart part = ROBLOX_PARTS[i];
		Engine::BasePart& newPart = *(new Engine::BasePart());
		newPart.SetParent(workspace);
		Engine::CFrame newCFrame(part.x,   part.y,   part.z,
					 part.r00, part.r01, part.r02,
					 part.r10, part.r11, part.r12,
					 part.r20, part.r21, part.r22
					);
		newPart.SetCFrame(newCFrame);
		switch (part.brick_color) {
			case 5:
				newPart.SetColor3(Engine::Color3(0.844, 0.773, 0.604));
				break;
			case 38:
				newPart.SetColor3(Engine::Color3(0.628, 0.373, 0.208));
				break;
			case 102:
				newPart.SetColor3(Engine::Color3(0.432, 0.6, 0.793));
				break;
			case 192:
				newPart.SetColor3(Engine::Color3(0.412, 0.251, 0.157));
				break;
			case 199:
				newPart.SetColor3(Engine::Color3(0.389, 0.373, 0.385));
				break;
			default:
				fprintf(stderr, "no brickcolor found: %d", part.brick_color);
				return -1;
		}
		newPart.SetSize(Engine::Vector3(part.size_x, part.size_y, part.size_z));
	}
	Engine::Renderer::Initialize(workspace);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	float lastX = 400.0f; 
	float lastY = 300.0f;
	bool firstMouse = true;

	glfwSetInputMode(mainWindow.GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!mainWindow.ShouldClose()) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		mainWindow.PollEvents();

		// --- Process Keyboard Inputs ---
		glm::vec3 directionDelta(0.0f);
		
		if (Engine::Input::IsKeyPressed(GLFW_KEY_W)) {
			directionDelta += camera.GetForwardVector();
		}
		if (Engine::Input::IsKeyPressed(GLFW_KEY_S)) {
			directionDelta -= camera.GetForwardVector();
		}
		if (Engine::Input::IsKeyPressed(GLFW_KEY_A)) {
			directionDelta -= camera.GetRightVector();
		}
		if (Engine::Input::IsKeyPressed(GLFW_KEY_D)) {
			directionDelta += camera.GetRightVector();
		}

		// Normalize the vector so diagonal movement isn't twice as fast
		if (glm::length(directionDelta) > 0.0f) {
			directionDelta = glm::normalize(directionDelta);
			camera.ProcessKeyboard(directionDelta, deltaTime);
		}

		// --- Process Mouse Look Movement ---
		double mouseX, mouseY;
		glfwGetCursorPos(mainWindow.GetNativeWindow(), &mouseX, &mouseY);

		if (firstMouse) {
			lastX = static_cast<float>(mouseX);
			lastY = static_cast<float>(mouseY);
			firstMouse = false;
		}

		float xOffset = static_cast<float>(mouseX) - lastX;
		float yOffset = lastY - static_cast<float>(mouseY); // Reversed: Y-coordinates go from bottom to top

		lastX = static_cast<float>(mouseX);
		lastY = static_cast<float>(mouseY);

		camera.ProcessMouseMovement(xOffset, yOffset, true);

		// --- Render Graphics Pipeline Pass ---
		Engine::Renderer::BeginFrame();

		Engine::Renderer::SetCamera(camera.GetViewMatrix(), camera.GetProjectionMatrix(mainWindow.GetAspectRatio()), camera.GetPosition());
		Engine::Renderer::Render();

		mainWindow.SwapBuffers();
	}

	Engine::Renderer::Shutdown();
	mainWindow.Close();
	return 0;
}
