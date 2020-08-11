#include "Renderer.hpp"

#include "GLDebug.hpp"
#include <gl/glew.h>

void Renderer::initialize()
{
	CHECK_GL(glEnable(GL_DEPTH_TEST));

	CHECK_GL(glEnable(GL_BLEND));
	CHECK_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	CHECK_GL(glEnable(GL_CULL_FACE));
	CHECK_GL(glCullFace(GL_BACK));
	CHECK_GL(glFrontFace(GL_CCW));

	CHECK_GL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)); // GL_LINE // GL_FILL
}

void Renderer::clear()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw(const Mesh& mesh)
{
	mesh.bind();
	CHECK_GL(glDrawElements(GL_TRIANGLES, mesh.indicesCount, GL_UNSIGNED_INT, nullptr));
}

void Renderer::draw(const Mesh& mesh, const Shader& shader)
{
	shader.bind();
	draw(mesh);
}

void Renderer::flush()
{
	glFlush();
}
