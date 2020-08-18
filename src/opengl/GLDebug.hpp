#pragma once

#include <gl/glew.h>

#ifdef DEVELOPMENT 

#define CHECK_GL(x)  clearGLErrors(); x; checkGLErrors();
void clearGLErrors();
void checkGLErrors();

#else

#define CHECK_GL(x) x;

#endif

