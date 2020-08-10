#pragma once

#include "basic/Fseconds.hpp"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct AsteroidSpawner
{
	Fseconds spawnInterval = std::chrono::seconds(1);
	Fseconds dt = Fseconds::zero();
};