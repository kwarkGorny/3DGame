#include "Renderer.hpp"

#include "GLDebug.hpp"

#include <gl/glew.h>
#include <stb/stb_image.h>
#include <array>

namespace
{
	constexpr std::array quadVertices = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int g_QuadVA;
	unsigned int g_QuadVB;
	//unsigned int g_QuadVI;

}

void Renderer::initialize()
{
	stbi_set_flip_vertically_on_load(1);

	CHECK_GL(glEnable(GL_DEPTH_TEST));

	CHECK_GL(glEnable(GL_BLEND));
	CHECK_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	CHECK_GL(glEnable(GL_CULL_FACE));
	CHECK_GL(glCullFace(GL_BACK));
	CHECK_GL(glFrontFace(GL_CCW));


	CHECK_GL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)); // GL_LINE // GL_FILL

	glGenVertexArrays(1, &g_QuadVA);
	glBindVertexArray(g_QuadVA);
	glGenBuffers(1, &g_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, g_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), quadVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Renderer::deinitialize()
{
	glDeleteBuffers(1, &g_QuadVA);
	glDeleteBuffers(1, &g_QuadVB);
}

void Renderer::clear()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void Renderer::drawQuad()
{
	glBindVertexArray(g_QuadVA);
	glDrawArrays(GL_TRIANGLES, 0, quadVertices.size());
}

void Renderer::flush()
{
	glFlush();
}
