#include "PlayerSystem.hpp"

#include "admins/InputAdmin.hpp"

#include "components/Dynamic.hpp"
#include "components/Player.hpp"

#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	}
}