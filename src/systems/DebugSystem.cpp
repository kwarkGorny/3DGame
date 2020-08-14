#include "DebugSystem.hpp"

#include "admins/InputAdmin.hpp"

#include "components/Transform.hpp"
#include "components/Player.hpp"

#include "basic/Logger.hpp"

#include <entt/entity/registry.hpp>

namespace DebugSystem
{
	void update(entt::registry& registry)
	{
		if (g_InputAdmin.isKeyPressed(SDLK_F4))
		{
			const auto mPos = g_InputAdmin.getMousePosition();
			logger::info("mouse pos x: {}, y: {}", mPos.x, mPos.y);
		}
		if (g_InputAdmin.isKeyPressed(SDLK_F5))
		{
			const auto players = registry.view<Player, Transform>();
			for (const auto player : players)
			{
				const auto& pos = players.get<Transform>(player).position;
				logger::info("player {} pos x: {}, y: {}, z: {}", player, pos.x, pos.y, pos.z);
			}
		}
	}
}