#include "CollisionSystem.hpp"

#include "basic/Logger.hpp"

#include "components/Asteroid.hpp"
#include "components/Bullet.hpp"
#include "components/Colliders.hpp"
#include "components/Obsolete.hpp"
#include "components/Player.hpp"
#include "components/Transform.hpp"

#include <entt/entity/registry.hpp>

namespace
{
	inline bool isCollidingABBA(const glm::vec3& posl, const RectCollider& rectl, const glm::vec3& posr, const RectCollider&rectr)
	{
		const glm::vec2 offsetL = {
			posl.x + rectl.pos.x,
			posl.y + rectl.pos.y
		};
		const glm::vec2 offsetR = {
			posr.x + rectr.pos.x,
			posr.y + rectr.pos.y
		};
		return offsetL.x < offsetR.x + rectr.size.x &&
			offsetL.x + rectl.size.x > offsetR.x &&
			offsetL.y < offsetR.y + rectr.size.y &&
			offsetL.y + rectl.size.y > offsetR.y;
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
					logger::info("asteroid bullet collision");
					registry.emplace_or_replace<Obsolete>(bullet);
					registry.emplace_or_replace<Obsolete>(asteroid);
				}
			}

			for (const auto player : players)
			{
				const auto& playerPos = bullets.get<Transform>(player).position;
				const auto& playerCollider = bullets.get<RectCollider>(player);
				if (isCollidingABBA(playerPos, playerCollider, asteroidPos, asteroidCollider))
				{
					logger::info("asteroid bullet player");
					registry.emplace_or_replace<Obsolete>(player);
					registry.emplace_or_replace<Obsolete>(asteroid);
				}
			}
		}
	}
}