#pragma once

#include <entt/entity/fwd.hpp>

union SDL_Event;

namespace PostEffectsSystem
{
	void handleEvent(entt::registry& registry, const SDL_Event& event);
}
