#include "FrameBuffer.hpp"

#include "GLDebug.hpp"

#include "basic/Logger.hpp"

FrameBuffer::FrameBuffer(int width, int height)
	: m_Width(width)
	, m_Height(height)
{
	glGenFramebuffers(1, &m_Id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Id);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId, 0);

	glGenRenderbuffers(1, &m_ObjectId);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ObjectId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_ObjectId);

	logger::error(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Framebuffer not completed");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_Id);
}

void FrameBuffer::bind() const
{
	CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, m_Id));
}

void FrameBuffer::unbind() const
{
	CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::bindTexture()const
{
	CHECK_GL(glActiveTexture(GL_TEXTURE0));
	CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_TextureId));
}

void FrameBuffer::unbindTexture() const
{
	CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));
}