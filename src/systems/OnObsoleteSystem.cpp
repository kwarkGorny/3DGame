#include "ObsoleteSystem.hpp"

#include "components/OnObsolete.hpp"
#include "components/Obsolete.hpp"

#include <entt/entity/registry.hpp>

namespace OnObsoleteSystem
{
	void update(entt::registry& registry)
	{
		const auto view = registry.view<Obsolete, OnObsolete>();
		for (const auto entity : view)
		{
			view.get<OnObsolete>(entity).callback(registry, entity);
		}
	}
}