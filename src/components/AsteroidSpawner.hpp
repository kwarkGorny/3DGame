#pragma once

#include "basic/Timer.hpp"

#include <glm/vec2.hpp>
#include <glm/ext/scalar_constants.hpp>

struct AsteroidSpawner
{
	Timer timer;
	float asteroidFrequency = 1;
	float asteroidFrequencyIncrease = 1;
	glm::vec2 omegaRange = { glm::pi<float>(), glm::pi<float>() };
};