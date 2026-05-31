#include "Shader.hpp"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>

namespace Engine {

Shader::Shader() {}
Shader::~Shader() {}
void Shader::Initalize(const char* vertexSource, const char* fragmentSource) {
	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		fprintf(stderr, "vertex shader compilation failed");
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		fprintf(stderr, "fragment shader compilation failed");
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, nullptr, infoLog);
		fprintf(stderr, "shader program linking failed");
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	compilesuccess = true;
}

void Shader::use() const {
	glUseProgram(ID);
}

void Shader::setMat4(const char* name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
}
void Shader::SetInt(const char* name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name), value);
}
void Shader::SetVec3(const char* name, const glm::vec3& value) const {
	glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}

} // namespace Engine

