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
		const auto entities = registry.view<Kinetic, Transform>();
		for (const auto entity : entities)
		{
			auto& t = entities.get<Transform>(entity);
			const auto& k = entities.get<Kinetic>(entity);
			t.transform = glm::translate(t.transform, k.velocity * dt.count());
		}
	}
}