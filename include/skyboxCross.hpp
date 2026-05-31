#pragma once
// 24 vertices (6 faces * 4 vertices)
// Layout: Position (x, y, z), TexCoords (u, v)
constexpr float skyboxCrossVertices[] = {
	// --- Front Face ---
	-0.5f, -0.5f, -0.5f,  0.25f, 1.0f/3.0f, // Bottom-Left
	 0.5f, -0.5f, -0.5f,  0.50f, 1.0f/3.0f, // Bottom-Right
	 0.5f,  0.5f, -0.5f,  0.50f, 2.0f/3.0f, // Top-Right
	-0.5f,  0.5f, -0.5f,  0.25f, 2.0f/3.0f, // Top-Left

	// --- Back Face ---
	 0.5f, -0.5f,  0.5f,  0.75f, 1.0f/3.0f, // Bottom-Left (from inside perspective)
	-0.5f, -0.5f,  0.5f,  1.00f, 1.0f/3.0f, // Bottom-Right
	-0.5f,  0.5f,  0.5f,  1.00f, 2.0f/3.0f, // Top-Right
	 0.5f,  0.5f,  0.5f,  0.75f, 2.0f/3.0f, // Top-Left

	// --- Left Face ---
	-0.5f, -0.5f,  0.5f,  0.00f, 1.0f/3.0f, // Bottom-Left
	-0.5f, -0.5f, -0.5f,  0.25f, 1.0f/3.0f, // Bottom-Right
	-0.5f,  0.5f, -0.5f,  0.25f, 2.0f/3.0f, // Top-Right
	-0.5f,  0.5f,  0.5f,  0.00f, 2.0f/3.0f, // Top-Left

	// --- Right Face ---
	 0.5f, -0.5f, -0.5f,  0.50f, 1.0f/3.0f, // Bottom-Left
	 0.5f, -0.5f,  0.5f,  0.75f, 1.0f/3.0f, // Bottom-Right
	 0.5f,  0.5f,  0.5f,  0.75f, 2.0f/3.0f, // Top-Right
	 0.5f,  0.5f, -0.5f,  0.50f, 2.0f/3.0f, // Top-Left

	// --- Top Face ---
	-0.5f,  0.5f, -0.5f,  0.25f, 2.0f/3.0f, // Bottom-Left
	 0.5f,  0.5f, -0.5f,  0.50f, 2.0f/3.0f, // Bottom-Right
	 0.5f,  0.5f,  0.5f,  0.50f, 1.0000f, // Top-Right
	-0.5f,  0.5f,  0.5f,  0.25f, 1.0000f, // Top-Left

	// --- Bottom Face ---
	-0.5f, -0.5f,  0.5f,  0.25f, 0.0000f, // Bottom-Left
	 0.5f, -0.5f,  0.5f,  0.50f, 0.0000f, // Bottom-Right
	 0.5f, -0.5f, -0.5f,  0.50f, 1.0f/3.0f, // Top-Right
	-0.5f, -0.5f, -0.5f,  0.25f, 1.0f/3.0f  // Top-Left
};

constexpr unsigned int skyboxCrossIndices[] = {
	0,  2,  1,    0,  3,  2,  // Front
	4,  6,  5,    4,  7,  6,  // Back
	8,  10, 9,    8,  11, 10, // Left
	12, 14, 13,   12, 15, 14, // Right
	16, 18, 17,   16, 19, 18, // Top
	20, 22, 21,   20, 23, 22  // Bottom
};
