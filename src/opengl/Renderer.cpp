#include "Renderer.hpp"

#include "GLDebug.hpp"
#include <gl/glew.h>

void Renderer::clear()const
{
	CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
	shader.bind();
	vertexArray.bind();
	indexBuffer.bind();
	CHECK_GL(glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));
}
