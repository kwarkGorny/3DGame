#pragma once

#include "VertexBuffer.hpp"
#include "VertexLayout.hpp"

class VertexArray
{
public:
	VertexArray();

	VertexArray(VertexArray&)noexcept = delete;
	VertexArray& operator=(VertexArray&)noexcept = delete;

	~VertexArray();

	void add(const VertexBuffer& buffer, const VertexLayout& layout);

	void bind()const;
	void unbind()const;

private:
	unsigned int m_Id;
};