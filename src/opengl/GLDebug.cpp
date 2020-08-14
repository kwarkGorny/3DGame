#include "GLDebug.hpp"

#ifdef DEVELOPMENT 
#include "basic/Logger.hpp"
#include <cassert>

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
		logger::error("OPENGL code:{}", error);
		assert(false);
		error = glGetError();
	}
}
#endif