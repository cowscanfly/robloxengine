#pragma once
// 24 unique vertices (6 faces * 4 vertices)
// Layout: Position (x, y, z), Normal (nx, ny, nz)
constexpr float cubeVertices[] = {
	// Front Face (Normals point straight down Z+)
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 0: Bottom-Left
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 1: Bottom-Right
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 2: Top-Right
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 3: Top-Left

	// Back Face (Normals point straight down Z-)
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // 4: Bottom-Right
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // 5: Bottom-Left
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // 6: Top-Left
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // 7: Top-Right

	// Left Face (Normals point straight down X-)
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // 8:  Bottom-Left
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // 9:  Bottom-Right
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // 10: Top-Right
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // 11: Top-Left

	// Right Face (Normals point straight down X+)
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // 12: Bottom-Left
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // 13: Bottom-Right
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // 14: Top-Right
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // 15: Top-Left

	// Top Face (Normals point straight down Y+)
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // 16: Bottom-Left
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // 17: Bottom-Right
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // 18: Top-Right
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // 19: Top-Left

	// Bottom Face (Normals point straight down Y-)
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // 20: Bottom-Left
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // 21: Bottom-Right
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // 22: Top-Right
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f  // 23: Top-Left
};

// 6 faces * 2 triangles * 3 indices = 36 indices total
constexpr unsigned int cubeIndices[] = {
	0,  1,  2,    2,  3,  0,  // Front
	4,  5,  6,    6,  7,  4,  // Back
	8,  9,  10,   10, 11, 8,  // Left
	12, 13, 14,   14, 15, 12, // Right
	16, 17, 18,   18, 19, 16, // Top
	20, 21, 22,   22, 23, 20  // Bottom
};
