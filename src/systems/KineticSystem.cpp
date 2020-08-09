#include "KineticSystem.hpp"

#include "components/Kinetic.hpp"
#include "components/Transform.hpp"


#include "opengl/Renderer.hpp"

#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace KineticSystem
{
	void update(entt::registry& registry, Fseconds dt)
	{
		const auto dtCount = dt.count();
		const auto entities = registry.view<Kinetic, Transform>();
		for (const auto entity : entities)
		{
			auto& t = entities.get<Transform>(entity);
			const auto& k = entities.get<Kinetic>(entity);
			t.position += k.velocity * dtCount;
			t.rotation += k.omega * dtCount;
		}
	}
}