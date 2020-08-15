#include "KineticSystem.hpp"

#include "basic/Logger.hpp"

#include "admins/RandomAdmin.hpp"

#include "components/AsteroidSpawner.hpp"

#include "contexts/SceneBound.hpp"

#include "utils/EntitiesUtils.hpp"

#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace AsteroidSpawnerSystem
{
	void update(entt::registry& registry, Fseconds dt)
	{
		const auto sceneBoundPtr = registry.try_ctx<SceneBound>();
		if (!sceneBoundPtr)
		{
			return;
		}
		const auto& sceneBound = *sceneBoundPtr;
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
			logger::info("current asteroids spawn frequency {}, increase {}, delay {}", spawner.asteroidFrequency, spawner.asteroidFrequencyIncrease, spawner.timer.duration.count());
			const glm::vec3 randomOmega = g_RandomAdmin.getDirection() * g_RandomAdmin.getUniform(spawner.omegaRange);
			const glm::vec3 randomPosition = { g_RandomAdmin.getUniform(sceneBound.start.x, sceneBound.end.x), sceneBound.end.y, 0.f };
			createAsteroid(registry, g_RandomAdmin.getUniform(1, 3), randomPosition, randomOmega);
		}
	}
}