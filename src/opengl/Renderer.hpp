#pragma once
#include <gl/glew.h>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer
{
public:

	void clear()const;

	void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader)const;
};