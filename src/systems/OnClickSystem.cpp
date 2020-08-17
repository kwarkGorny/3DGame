#include "OnClickSystem.hpp"

#include "admins/InputAdmin.hpp"

#include "components/Colliders.hpp"
#include "components/OnClick.hpp"
#include "components/Transform.hpp"

#include <entt/entity/registry.hpp>
#include <SDL_events.h>

namespace OnClickSystem
{
	void handleEvent(entt::registry& registry, const SDL_Event& event)
	{
		if (event.type == SDL_MOUSEBUTTONUP)
		{
			const auto mouse = g_InputAdmin.getMousePosition();
			const auto view = registry.view<OnClick, Rect2DCollider, Transform>();
			for (const auto entity : view)
			{
				const auto& position = view.get<Transform>(entity).position;
				const auto& rect = view.get<Rect2DCollider>(entity);
				if (rect.intersects(mouse.x - position.x, mouse.y - position.y))
				{
					view.get<OnClick>(entity).callback(registry, entity);
					break;
				}
			}
		}
	}
}
