#pragma once

#include <glad/glad.h>

namespace Engine {

class Texture {
public:
	Texture();
	~Texture() = default;

	bool Initialize(const char* filePath, bool flipY = true);
	void Destroy();

	void Bind(GLuint slot = 0) const;
	void Unbind() const;

	GLuint GetID() const { return m_ID; }
	int GetWidth() const       { return m_Width; }
	int GetHeight() const      { return m_Height; }

private:
	// Prevent accidental copies which would cause double-deletion crashes
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	GLuint m_ID;
	int m_Width;
	int m_Height;
	int m_Channels;
};

} // namespace Engine
