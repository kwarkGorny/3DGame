#pragma once

#include "basic/Fseconds.hpp"
#include <entt/entity/fwd.hpp>

namespace AsteroidSpawnerSystem
{
	void update(entt::registry& registry, Fseconds dt);
}