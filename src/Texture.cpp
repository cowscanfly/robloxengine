#include "Texture.hpp"

#include <glad/glad.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb_image.h"

namespace Engine {

Texture::Texture() 
	: m_ID(0)
	, m_Width(0)
	, m_Height(0)
	, m_Channels(0) 
{}

bool Texture::Initialize(const char* filePath, bool flipY) {
	// Tell stb_image to flip the image vertically on load 
	// (OpenGL's Y-axis starts from the bottom, but images start from the top)
	stbi_set_flip_vertically_on_load(flipY);

	unsigned char* data = stbi_load(filePath, &m_Width, &m_Height, &m_Channels, 0);
	if (!data) {
		fprintf(stderr, "failed to load texture file, %s", filePath);
		return false;
	}
	printf("SUCCESS: Loaded %s (%dx%d pixels, %d channels)\n", filePath, m_Width, m_Height, m_Channels);

	GLenum internalFormat = 0;
	GLenum dataFormat = 0;

	if (m_Channels == 1) {
		internalFormat = GL_RED;
		dataFormat = GL_RED;
	} else if (m_Channels == 3) {
		internalFormat = GL_RGB8; 
		dataFormat = GL_RGB;
	} else if (m_Channels == 4) {
		internalFormat = GL_RGBA8; 
		dataFormat = GL_RGBA;
	}

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Essential edge wrapping filters (prevents lines along seams)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Linear interpolation filters for clean, non-pixelated up/down scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
	// glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void Texture::Destroy() {
	if (m_ID != 0) {
		glDeleteTextures(1, &m_ID);
		m_ID = 0;
	}
}

void Texture::Bind(GLuint slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace Engine

