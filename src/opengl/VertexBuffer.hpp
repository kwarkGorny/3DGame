#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);

	VertexBuffer(VertexBuffer&)noexcept = delete;
	VertexBuffer& operator=(VertexBuffer&)noexcept = delete;

	~VertexBuffer();

	void bind()const;
	void unbind()const;

private:
	unsigned int m_Id;
};