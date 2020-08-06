#include "VertexBuffer.hpp"

#include "GLDebug.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	CHECK_GL(glGenBuffers(1, &m_Id));
	CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
	CHECK_GL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	CHECK_GL(glDeleteBuffers(1, &m_Id));
}

void VertexBuffer::bind()const
{
	CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
}

void VertexBuffer::unbind()const
{
	CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
