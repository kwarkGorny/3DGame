#include "KineticSystem.hpp"

#include "components/Kinetic.hpp"
#include "components/Transform.hpp"

#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace KineticSystem
{
	void update(entt::registry& registry, Fseconds dt)
	{
		const auto dtCount = dt.count();

		const auto constrainedKinetics = registry.view<KineticConstraint, Kinetic>();

		for (const auto entity : constrainedKinetics)
		{
			const auto& ck = constrainedKinetics.get<KineticConstraint>(entity);
			auto& k = constrainedKinetics.get<Kinetic>(entity);
			if (const auto vLength = glm::length(k.velocity); vLength > ck.maxVelocity)
			{
				k.velocity *= ck.maxVelocity / vLength;
			}
			if (const auto oLength = glm::length(k.omega); oLength > ck.maxOmega)
			{
				k.omega *= ck.maxOmega / oLength;
			}
		}

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