#pragma once

#include "opengl/Texture.hpp"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct Renderable2D
{
	SharedTexture texture = nullptr;
	glm::vec4 color = glm::vec4(1.f);
};
