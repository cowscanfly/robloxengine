#pragma once
#include "Window.hpp" 
#include <GLFW/glfw3.h>

namespace Engine {
namespace Input {
	void Initialize(Window* targetWindow); 

	bool IsKeyPressed(int keyCode); 

	void GetMousePosition(double& outX, double& outY); 

} // namespace Input
} // namespace Engine
