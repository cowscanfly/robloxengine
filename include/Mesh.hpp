#pragma once
#include <stddef.h>
#include <glad/glad.h>
namespace Engine {

class Mesh {
public:
	Mesh();
	void Initalize(const float* vertices, size_t vertexSize, const GLuint* indices, size_t indexSize, GLenum usage); 
	
	~Mesh(); 

	void Destroy(); 

	GLuint GetVAO() const; 
	GLuint GetVBO() const; 
	GLuint GetEBO() const; 

	GLuint GetIndexCount() const; 
private:
	// disble copying
	Mesh(const Mesh&);
	Mesh& operator=(const Mesh&);

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	GLuint m_IndexCount;
};

} // namespace Engine
