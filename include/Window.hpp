#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

namespace Engine {

class Window {
public:
	Window(unsigned int width, unsigned int height, const char* title); 
	~Window(); 

	bool Initialize(); 
	void SwapBuffers() const; 
	void PollEvents() const; 
	void Close(); 

	bool ShouldClose() const; 
	unsigned int GetWidth() const; 
	unsigned int GetHeight() const; 
	float GetAspectRatio() const; 
	
	GLFWwindow* GetNativeWindow() const; 

private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	// disable copying
	Window(const Window&);
	Window& operator=(const Window&);

	GLFWwindow* m_Window;
	unsigned int m_Width;
	unsigned int m_Height;
	const char* m_Title;
	bool m_IsInitialized;
};

} // namespace Engine
