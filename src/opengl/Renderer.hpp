#pragma once
#include <gl/glew.h>

#include "Texture.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

class Renderer
{
public:
	static void initialize();

	static void clear();

	static void draw(const Mesh& mesh);

	static void draw(const Mesh& mesh, const Shader& shader);

	static void drawQuad(const glm::vec4& position, const glm::vec4& size, const glm::vec4& color);

	static void drawQuad(const glm::vec4& position, const glm::vec4& size, unsigned int textureId);

	static void flush();

};