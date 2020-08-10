#include "KineticSystem.hpp"

#include "components/AsteroidSpawner.hpp"

#include "utils/EntitiesUtils.hpp"

#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace AsteroidSpawnerSystem
{
	void update(entt::registry& registry, Fseconds dt)
	{
		const auto spawners = registry.view<AsteroidSpawner>();
		for (const auto entity : spawners)
		{
			auto& spawner = spawners.get(entity);
			if (spawner.timer.update(dt))
			{
				createAsteroid(registry);
			}
		}
	}
}