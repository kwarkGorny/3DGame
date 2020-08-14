#include "components/Transform.hpp"
#include "components/Kinetic.hpp"
#include "components/Weapon.hpp"

#include "utils/EntitiesUtils.hpp"

#pragma warning(push, 0) 
#include <entt/entity/registry.hpp>
#pragma warning(pop)

namespace WeaponSystem
{
	void update(entt::registry& registry, Fseconds dt)
	{
		const auto weaponsView = registry.view<Weapon, Transform>();
		for (const auto entity : weaponsView)
		{
			auto& weapon = weaponsView.get<Weapon>(entity);
			if (weapon.timer.update(dt))
			{
				auto transform = weaponsView.get<Transform>(entity);
				transform.position += weapon.offset;
				createBullet(registry, transform, { glm::vec3(0.f, weapon.bulletVelocity, 0.f) });
			}
		}
	}
}