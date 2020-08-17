#pragma once

#include <gl/glew.h>

class Renderer
{
public:
	static void initialize();

	static void deinitialize();

	static void clear();

	static void drawQuad();

	static void flush();

};