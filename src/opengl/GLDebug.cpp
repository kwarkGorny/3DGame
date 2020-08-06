#include "GLDebug.hpp"

#include <gl/glew.h>
#include <cassert>
#include <iostream>

void clearGLErrors()
{
	while (glGetError() != GL_NO_ERROR)
	{
		assert(false);
	}
}

void checkGLErrors()
{
	auto error = glGetError();
	while (error != GL_NO_ERROR)
	{
		std::cout << "[OPENGL ERROR] code: " << error << '\n';
		assert(false);
		error = glGetError();
	}
}