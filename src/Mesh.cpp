#include "Mesh.hpp"
#include <stddef.h>
#include <glad/glad.h>

namespace Engine {

Mesh::Mesh() {}
void Mesh::Initalize(const float* vertices, size_t vertexSize, const GLuint* indices, size_t indexSize, GLenum usage) {
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, usage);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, usage);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_IndexCount = indexSize / sizeof(GLuint);
}

Mesh::~Mesh() {}

void Mesh::Destroy() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO); 
}

GLuint Mesh::GetVAO() const {
	return m_VAO;
}
GLuint Mesh::GetVBO() const {
	return m_VBO;
}
GLuint Mesh::GetEBO() const {
	return m_EBO;
}

GLuint Mesh::GetIndexCount() const {
	return m_IndexCount;
}

} // namespace Engine

