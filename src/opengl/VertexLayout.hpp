#pragma once

#include <vector>
#include <gl/glew.h>

struct VertexLayoutElement
{
	unsigned int type;
	unsigned int count;
	unsigned int normalized;
	unsigned int size;
};


class VertexLayout
{
public:
	VertexLayout() = default;

	VertexLayout(VertexLayout&)noexcept = delete;
	VertexLayout& operator=(VertexLayout&)noexcept = delete;

	template<class T>
	void push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		constexpr auto size = sizeof(GL_FLOAT);
		m_Elements.push_back({GL_FLOAT, count, GL_FALSE, size});
		m_Stride += size * count;
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		constexpr auto size = sizeof(GL_UNSIGNED_INT);
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, size});
		m_Stride += size * count;
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		constexpr auto size = sizeof(GL_UNSIGNED_BYTE);
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE, size});
		m_Stride += size * count;
	}

	inline const auto& getElements()const { return m_Elements; }
	inline auto getStride()const { return m_Stride; }

private:
	std::vector<VertexLayoutElement> m_Elements;
	unsigned int m_Stride = 0;
};