#pragma once

#include "basic/Timer.hpp"

#include <glm/vec3.hpp>

struct Weapon
{
	Timer timer;
	glm::vec3 offset = glm::vec3(0.f);
	float bulletVelocity = 10.f;
};