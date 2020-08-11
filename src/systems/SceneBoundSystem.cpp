#include "SceneBoundSystem.hpp"

#include "components/Transform.hpp"
#include "components/SceneBound.hpp"
#include "components/Obsolete.hpp"

#include "contexts/SceneSize.hpp"

#include <entt/entity/registry.hpp>

namespace SceneBoundSystem
{
	void update(entt::registry& registry)
	{
		const auto sceneSizePtr = registry.try_ctx<SceneSize>();
		if(!sceneSizePtr)
		{
			return;
		}
		const auto& size = sceneSizePtr->size;
		const auto view = registry.view<SceneBound, Transform>();
		for (const auto entity : view)
		{
			const auto& position = view.get<Transform>(entity).position;
			if (position.x < 0 || position.x > size.x ||
				position.y < 0 || position.y > size.y)
			{
				registry.emplace<Obsolete>(entity);
			}
		}
	}
}