#pragma once

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
		const auto weaponsView = registry.view<Weapon>();
		for (const auto entity : weaponsView)
		{
			auto& weapon = weaponsView.get(entity);
			if (weapon.timer.update(dt))
			{
				createBullet(registry);
			}
		}
	}
}