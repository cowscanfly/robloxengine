#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in mat4 instanceMatrix;
layout (location = 6) in vec3 instanceColor;

out vec3 FragPos;  // This MUST be sent to the fragment shader
out vec3 Normal;   // This MUST be sent to the fragment shader
out vec3 Color;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	// FIX: Calculate the exact world space position of this vertex
	FragPos = vec3(instanceMatrix * vec4(aPos, 1.0));
	
	// Pass the normal to world space
	Normal = mat3(transpose(inverse(instanceMatrix))) * aNormal;  
	Color = instanceColor;
	
	// Set the final clip-space position for the rasterizer
	gl_Position = projection * view * vec4(FragPos, 1.0);
}
