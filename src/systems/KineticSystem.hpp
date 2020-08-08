#pragma once

#include "basic/Fseconds.hpp"
#include <entt/entity/fwd.hpp>

namespace KineticSystem
{
	void update(entt::registry& registry, Fseconds dt);
}