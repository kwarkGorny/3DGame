#include "PlayerSystem.hpp"

#include "admins/InputAdmin.hpp"

#include "components/Dynamic.hpp"
#include "components/Kinetic.hpp"
#include "components/Player.hpp"
#include "components/Transform.hpp"
#include "components/Colliders.hpp"

#include "contexts/SceneBound.hpp"

#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <utility>

namespace PlayerSystem
{
	void update(entt::registry& registry)
	{

		glm::vec3 inputForce = { 0.f, 0.f, 0.f };
		if (g_InputAdmin.isKeyPressed(SDLK_a))
		{
			inputForce.x += -1.f;
		}
		else if (g_InputAdmin.isKeyPressed(SDLK_d))
		{
			inputForce.x += 1.f;
		}
		const auto view = registry.view<Player, Dynamic>();
		for (const auto e : view)
		{
			auto& p = view.get<Player>(e);
			auto& d = view.get<Dynamic>(e);
			d.force = inputForce * p.manoeuveringEnginesThrust;
		}
		const auto sceneBoundPtr = registry.try_ctx<SceneBound>();
		if (!sceneBoundPtr)
		{
			return;
		}
		const auto& bound = *sceneBoundPtr;

		const auto playersView = registry.view<Player, RectCollider, Transform, Kinetic, Dynamic>();
		for (const auto entity : playersView)
		{
			auto& pos = playersView.get<Transform>(entity).position;
			const auto rect = playersView.get<RectCollider>(entity);
			if (pos.x + rect.pos.x < bound.start.x || pos.x + rect.pos.x + rect.size.x > bound.end.x ||
				pos.y + rect.pos.y < bound.start.y || pos.y + rect.pos.y + rect.size.y > bound.end.y)
			{
				pos.x = std::clamp(pos.x, bound.start.x - rect.pos.x, bound.end.x - rect.size.x - rect.pos.x);
				pos.y = std::clamp(pos.y, bound.start.y - rect.pos.y, bound.end.y - rect.size.y - rect.pos.y);
				playersView.get<Dynamic>(entity).force = {};
				playersView.get<Kinetic>(entity).velocity = {};
			}
		}
	}
}