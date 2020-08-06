#include "Texture.hpp"

#include "GLDebug.hpp"

#include <stb/stb_image.h>

Texture::Texture(const std::string& filePath)
{
	CHECK_GL(glGenTextures(1, &m_Id));
	CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_Id));
	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	stbi_set_flip_vertically_on_load(1);
	auto buffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
	if (buffer)
	{
		stbi_image_free(buffer);
	}
	unbind();
}

Texture::~Texture()
{
	CHECK_GL(glDeleteTextures(1, &m_Id));
}

void Texture::bind(unsigned int slot) const
{
	CHECK_GL(glActiveTexture(GL_TEXTURE0 + slot));
	CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_Id));
}

void Texture::unbind() const
{
	CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));
}
