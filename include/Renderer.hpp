#pragma once

#include "Instances/Workspace.hpp"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {
namespace Renderer {
	void Initialize(Workspace* workspace); 
	void Shutdown(); 
	void BeginFrame(); 
	void Render(); 

	void SetCamera(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos); 
} // namespace Renderer
} // namespace Engine
