#include "IndexBuffer.hpp"

#include "GLDebug.hpp"

IndexBuffer::IndexBuffer(const unsigned int* indexes, unsigned int count)
	: m_Count(count)
{
	CHECK_GL(glGenBuffers(1, &m_Id));
	CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
	CHECK_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indexes, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	CHECK_GL(glDeleteBuffers(1, &m_Id));
}

void IndexBuffer::bind()const
{
	CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
}

void IndexBuffer::unbind()const
{
	CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
