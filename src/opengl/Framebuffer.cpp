#include "FrameBuffer.hpp"

#include "GLDebug.hpp"

#include "opengl/Primitives.hpp"

#include "basic/Logger.hpp"

FrameBuffer::FrameBuffer(int width, int height)
	: m_Width(width)
	, m_Height(height)
{
	glGenFramebuffers(1, &m_Id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Id);

	CHECK_GL(glGenTextures(1, &m_TextureId));
	CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_TextureId));

	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId, 0);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_TextureId, 0);

	logger::error(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Framebuffer not completed");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_Id);
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
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