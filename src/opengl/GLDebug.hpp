#pragma once
#include <gl/glew.h>

#define CHECK_GL(x)  clearGLErrors(); x; checkGLErrors();

void clearGLErrors();
void checkGLErrors();
