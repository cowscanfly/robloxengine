#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main() {
	// 1. SAFE UV EDGE BUFFER MATH
	// Calculate which 512x512 tile grid slot this UV coordinate belongs to (4 columns, 3 rows)
	vec2 tile = floor(aTexCoords * vec2(4.0, 3.0));
	
	// Find the exact center coordinate of this specific face tile
	vec2 tileCenter = (tile + vec2(0.5)) / vec2(4.0, 3.0);
	
	// Shrink the UV coordinates slightly inward (0.998) toward the safe center pixel zone
	// This pulls the sampling boundary away from the transparent atlas gap edges
	TexCoords = tileCenter + (aTexCoords - tileCenter) * 0.998;
	
	// 2. CAMERA PERSPECTIVE & VIEW MATRIX MATH
	// Multiply by projection and view to get the correct 3D tracking orientation
	vec4 pos = projection * view * vec4(aPos, 1.0);
	
	// Force depth value to 1.0 so it draws behind everything else in the scene
	gl_Position = pos.xyww; 
}
