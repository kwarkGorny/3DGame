#include "CollisionSystem.hpp"

#include "admins/SoundAdmin.hpp"

#include "basic/Logger.hpp"

#include "components/Asteroid.hpp"
#include "components/Bullet.hpp"
#include "components/Colliders.hpp"
#include "components/Obsolete.hpp"
#include "components/Player.hpp"
#include "components/Transform.hpp"
#include "components/Kinetic.hpp"

#include "utils/EntitiesUtils.hpp"

#include <entt/entity/registry.hpp>

namespace
{
	inline bool isCollidingABBA(const glm::vec3& posl, const RectCollider& rectl, const glm::vec3& posr, const RectCollider&rectr)
	{
		const glm::vec3 offsetL = {
			posl.x + rectl.pos.x,
			posl.y + rectl.pos.y,
			posl.z + rectl.pos.z
		};
		const glm::vec3 offsetR = {
			posr.x + rectr.pos.x,
			posr.y + rectr.pos.y,
			posr.z + rectr.pos.z
		};
		return offsetL.x < offsetR.x + rectr.size.x &&
			offsetL.x + rectl.size.x > offsetR.x &&
			offsetL.y < offsetR.y + rectr.size.y &&
			offsetL.y + rectl.size.y > offsetR.y &&
			offsetL.z < offsetR.z + rectr.size.z &&
			offsetL.z + rectl.size.z > offsetR.z;
	}
}

namespace CollisionSystem
{
	void update(entt::registry& registry)
	{
		const auto asteroids = registry.view<Asteroid, RectCollider, Transform>();
		const auto bullets = registry.view<Bullet, RectCollider, Transform>();
		const auto players= registry.view<Player, RectCollider, Transform>();

		for (const auto asteroid : asteroids)
		{
			const auto& asteroidPos = asteroids.get<Transform>(asteroid).position;
			const auto& asteroidCollider = asteroids.get<RectCollider>(asteroid);

			for (const auto bullet : bullets)
			{
				const auto& bulletPos = bullets.get<Transform>(bullet).position;
				const auto& bulletCollider = bullets.get<RectCollider>(bullet);
				if (isCollidingABBA(bulletPos, bulletCollider, asteroidPos, asteroidCollider))
				{
					const auto& asteroidData = asteroids.get<Asteroid>(asteroid);
					createAsteroidExplosion(registry, asteroidPos, asteroidData.explosionDuration);
					createBulletExplosion(registry, bulletPos);
					registry.emplace_or_replace<Obsolete>(bullet);
					registry.emplace_or_replace<Obsolete>(asteroid);
					g_SoundAdmin.play("data/sounds/explosion.wav");
				}
			}

			for (const auto player : players)
			{
				const auto& playerPos = players.get<Transform>(player).position;
				const auto& playerCollider = players.get<RectCollider>(player);
				if (isCollidingABBA(playerPos, playerCollider, asteroidPos, asteroidCollider))
				{
					const auto& asteroidData = asteroids.get<Asteroid>(asteroid);
					createAsteroidExplosion(registry, asteroidPos, asteroidData.explosionDuration);
					createPlayerExplosion(registry, playerPos);
					registry.emplace_or_replace<Obsolete>(player);
					registry.emplace_or_replace<Obsolete>(asteroid);
					g_SoundAdmin.play("data/sounds/explosion.wav");
				}
			}
		}
	}
}