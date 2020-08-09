#pragma once
#include <gl/glew.h>

#include "Shader.hpp"
#include "Mesh.hpp"

class Renderer
{
public:
	static void initialize();

	static void clear();

	static void draw(const Mesh& mesh);

	static void draw(const Mesh& mesh, const Shader& shader);

	static void flush();

};