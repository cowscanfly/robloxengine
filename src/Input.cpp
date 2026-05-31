#include "Input.hpp"
#include "Window.hpp" 
#include <GLFW/glfw3.h>

namespace Engine {
namespace Input {
	namespace {
		Window* g_WindowContext = nullptr;
	}

	void Initialize(Window* targetWindow) {
		g_WindowContext = targetWindow;
	}

	bool IsKeyPressed(int keyCode) {
		if (!g_WindowContext) return false;
		
		GLFWwindow* nativeWin = g_WindowContext->GetNativeWindow();
		return glfwGetKey(nativeWin, keyCode) == GLFW_PRESS;
	}

	void GetMousePosition(double& outX, double& outY) {
		if (!g_WindowContext) {
			outX = 0.0;
			outY = 0.0;
			return;
		}
		
		glfwGetCursorPos(g_WindowContext->GetNativeWindow(), &outX, &outY);
	}

} // namespace Input
} // namespace Engine

