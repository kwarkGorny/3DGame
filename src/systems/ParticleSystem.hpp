#pragma once

#include "basic/Fseconds.hpp"

#include <entt/entity/fwd.hpp>


namespace ParticleSystem
{
	void update(entt::registry& registry, Fseconds dt);

	void draw(entt::registry& registry);
}