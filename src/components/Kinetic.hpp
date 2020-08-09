#pragma once

#include <glm/vec3.hpp>

struct Kinetic
{
	glm::vec3 velocity = glm::vec3(0.f);
	glm::vec3 omega = glm::vec3(0.f);
};