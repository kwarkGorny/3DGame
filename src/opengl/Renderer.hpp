#pragma once
#include <gl/glew.h>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer
{
public:
	static void clear();

	static void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader);
};