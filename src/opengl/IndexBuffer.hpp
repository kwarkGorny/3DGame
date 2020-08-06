#pragma once
#include <cstdint>

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* indexes, unsigned int count);

	IndexBuffer(IndexBuffer&)noexcept = delete;
	IndexBuffer& operator=(IndexBuffer&)noexcept = delete;

	~IndexBuffer();

	void bind() const;
	void unbind() const;

	[[nodiscard]] unsigned int getCount()const { return m_Count; }
private:
	unsigned int m_Id;
	unsigned int m_Count;
};