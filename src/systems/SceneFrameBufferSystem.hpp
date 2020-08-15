#pragma once

#include <entt/entity/fwd.hpp>

namespace SceneFrameBufferSystem
{
	void predraw(entt::registry& registry);

	void postdraw(entt::registry& registry);
}