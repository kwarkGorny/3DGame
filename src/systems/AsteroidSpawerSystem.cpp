#include "KineticSystem.hpp"

#include "admins/RandomAdmin.hpp"

#include "components/AsteroidSpawner.hpp"

#include "contexts/SceneSize.hpp"

#include "utils/EntitiesUtils.hpp"

#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace AsteroidSpawnerSystem
{
	void update(entt::registry& registry, Fseconds dt)
	{
		const auto sceneSizePtr = registry.try_ctx<SceneSize>();
		if (!sceneSizePtr)
		{
			return;
		}
		const auto& sceneSize = sceneSizePtr->size;
		const auto spawners = registry.view<AsteroidSpawner>();
		for (const auto entity : spawners)
		{
			auto& spawner = spawners.get(entity);
			if (!spawner.timer.update(dt))
			{
				continue;
			}
			spawner.asteroidFrequency += spawner.asteroidFrequencyIncrease;
			spawner.timer.duration = Fseconds(1 / spawner.asteroidFrequency);
			const auto randomOmega = g_RandomAdmin.getDirection() * g_RandomAdmin.getUniform(spawner.omegaRange);
			const glm::vec2 randomPosition = { g_RandomAdmin.getUniform(0, sceneSize.x), sceneSize.y };
			createAsteroid(registry, g_RandomAdmin.getUniform(1, 3), randomPosition, randomOmega);
		}
	}
}