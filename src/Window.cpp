#include "Window.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>


namespace Engine {

Window::Window(unsigned int width, unsigned int height, const char* title) {
	m_Width = width;
	m_Height = height;
	m_Title = title;
	m_Window = nullptr;
	m_IsInitialized = false;
}
Window::~Window() {
	if (m_Window) {
		glfwDestroyWindow(m_Window);
	}
	glfwTerminate();
}

bool Window::Initialize() {
	if (!glfwInit()) {
		fprintf(stderr, "failed to initialize glfw\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
	if (m_Window == NULL) {
		fprintf(stderr, "failed to create glfw window\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_Window);
	
	glfwSetWindowUserPointer(m_Window, this);

	glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "failed to initialize GLAD\n");
		glfwTerminate();
		return false;
	}

	// disable vsync
	glfwSwapInterval(0);

	m_IsInitialized = true;
	return true;
}
void Window::SwapBuffers() const {
	glfwSwapBuffers(m_Window);
}
void Window::PollEvents() const {
	glfwPollEvents();
}
void Window::Close() {
	if (m_Window) {
		glfwDestroyWindow(m_Window);
	}
	glfwTerminate();
}

bool Window::ShouldClose() const {
	return glfwWindowShouldClose(m_Window);
}
unsigned int Window::GetWidth() const {
	return m_Width;
}
unsigned int Window::GetHeight() const {
	return m_Height;
}
float Window::GetAspectRatio() const {
	return (float)m_Width / (float)m_Height;
}

GLFWwindow* Window::GetNativeWindow() const {
	return m_Window;
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (instance) {
		instance->m_Width = static_cast<unsigned int>(width);
		instance->m_Height = static_cast<unsigned int>(height);
	}
}

} // namespace Engine

