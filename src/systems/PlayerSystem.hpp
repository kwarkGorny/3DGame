#pragma once

#include "basic/Fseconds.hpp"
#include <entt/entity/fwd.hpp>

union SDL_Event;

namespace PlayerSystem
{
	void update(entt::registry& registry);
}