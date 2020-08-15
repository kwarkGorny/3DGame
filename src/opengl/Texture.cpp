#include "Texture.hpp"

#include "basic/Logger.hpp"

#include "GLDebug.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

Texture::Texture(const std::string& filePath)
{
	CHECK_GL(glGenTextures(1, &m_Id));
	CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_Id));
	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	auto buffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	logger::warning(!buffer, "such texture dont exist {}", filePath);
	CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
	stbi_image_free(buffer);
	
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
