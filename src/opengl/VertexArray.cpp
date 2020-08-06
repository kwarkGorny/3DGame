#include "VertexArray.hpp"

#include "GLDebug.hpp"


VertexArray::VertexArray()
{
	CHECK_GL(glGenVertexArrays(1, &m_Id));
}

VertexArray::~VertexArray()
{
	CHECK_GL(glDeleteVertexArrays(1, &m_Id));
}

void VertexArray::add(const VertexBuffer& buffer, const VertexLayout& layout)
{
	bind();
	buffer.bind();
	const auto& elements = layout.getElements();
	const auto stride = layout.getStride();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		CHECK_GL(glEnableVertexAttribArray(i));
		CHECK_GL(glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (const void*)offset));
		offset += element.count * element.size;
	}
}

void VertexArray::bind() const
{
	CHECK_GL(glBindVertexArray(m_Id));
}

void VertexArray::unbind() const
{
	CHECK_GL(glBindVertexArray(0));
}
