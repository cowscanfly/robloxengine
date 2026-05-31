#pragma once

#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

class Shader {
public:
	GLuint ID; 
	bool compilesuccess = false;

	Shader();
	~Shader();
	void Initalize(const char* vertexSource, const char* fragmentSource); 

	void use() const; 

	void setMat4(const char* name, const glm::mat4& mat) const; 
	void SetInt(const char* name, int value) const; 
	void SetVec3(const char* name, const glm::vec3& value) const; 
};

} // namespace Engine
