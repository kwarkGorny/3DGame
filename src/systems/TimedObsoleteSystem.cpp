#include "TimedObsoleteSystem.hpp"

#include "components/Obsolete.hpp"
#include "components/TimedObsolete.hpp"

#include <entt/entity/registry.hpp>

namespace TimedObsoleteSystem
{
	void update(entt::registry& registry, Fseconds dt)
	{
		const auto view = registry.view<TimedObsolete>();
		for (const auto entity : view)
		{
			auto& timer = view.get(entity).timer;
			if (timer.update(dt))
			{
				registry.emplace_or_replace<Obsolete>(entity);
			}
		}
	}
}