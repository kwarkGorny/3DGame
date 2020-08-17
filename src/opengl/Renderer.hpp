#pragma once
#include <gl/glew.h>

#include "Texture.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

class Renderer
{
public:
	static void initialize();

	static void deinitialize();

	static void clear();

	static void draw(const Mesh& mesh);

	static void draw(const Mesh& mesh, const Shader& shader);

	static void drawQuad();

	static void flush();

};