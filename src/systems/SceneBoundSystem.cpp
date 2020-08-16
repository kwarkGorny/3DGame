#include "SceneBoundSystem.hpp"

#include "basic/Logger.hpp"

#include "components/Transform.hpp"
#include "components/Bounded.hpp"
#include "components/Obsolete.hpp"

#include "contexts/SceneBound.hpp"

#include <entt/entity/registry.hpp>

namespace SceneBoundSystem
{
	void update(entt::registry& registry)
	{
		const auto sceneBoundPtr = registry.try_ctx<SceneBound>();
		if(!sceneBoundPtr)
		{
			return;
		}
		const auto& bound = *sceneBoundPtr;
		const auto view = registry.view<Bounded, Transform>();
		for (const auto entity : view)
		{
			const auto& position = view.get<Transform>(entity).position;
			if (position.x < bound.start.x || position.x > bound.end.x ||
				position.y < bound.start.y || position.y > bound.end.y)
			{
				registry.emplace<Obsolete>(entity);
			}
		}
	}
}