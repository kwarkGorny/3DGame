#include "KineticSystem.hpp"

#include "components/Dynamic.hpp"
#include "components/Kinetic.hpp"

#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace DynamicSystem
{
	void update(entt::registry& registry, Fseconds dt)
	{
		const auto dtCount = dt.count();
		const auto entities = registry.view<Dynamic, Kinetic>();
		for (const auto entity : entities)
		{
			const auto& d = entities.get<Dynamic>(entity);
			auto& k = entities.get<Kinetic>(entity);
			if (d.mass > 0)
			{
				k.velocity += (d.force / d.mass) * dtCount;
			}
		}
	}
}