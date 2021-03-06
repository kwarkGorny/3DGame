#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Camera
{
	glm::vec3 position;
	glm::mat4 projection;
	glm::mat4 view;
};