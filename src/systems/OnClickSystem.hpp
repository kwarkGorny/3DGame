#pragma once

#include <entt/entity/fwd.hpp>

union SDL_Event;

namespace OnClickSystem
{
	void handleEvent(entt::registry& registry, const SDL_Event& event);
}
