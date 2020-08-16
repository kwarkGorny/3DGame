#include "admins/SoundAdmin.hpp"

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
				const auto& transform = weaponsView.get<Transform>(entity);
				createBullet(registry, transform.position + weapon.offset, { glm::vec3(0.f, weapon.bulletVelocity, 0.f), glm::vec3(0.f, 1.f, 0.f) });
				g_SoundAdmin.play("data/sounds/shoot.wav");
			}
		}
	}
}