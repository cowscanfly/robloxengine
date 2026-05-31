#include "Engine.hpp"
#include <GLFW/glfw3.h>

namespace {
    GLFWwindow* g_Window = nullptr;
    bool g_IsRunning = false;
}

bool Engine::Initialize() {
    if (!glfwInit()) return false;
    g_Window = glfwCreateWindow(800, 600, "Engine", NULL, NULL);
    g_IsRunning = (g_Window != nullptr);
    return g_IsRunning;
}

bool Engine::IsRunning() {
    return g_IsRunning && !glfwWindowShouldClose(g_Window);
}

void Engine::Shutdown() {
    glfwTerminate();
}
