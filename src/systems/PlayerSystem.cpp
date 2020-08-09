#include "KineticSystem.hpp"

#include "admins/InputAdmin.hpp"

#include "components/Kinetic.hpp"
#include "components/Player.hpp"

#include <SDL_events.h>
#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PlayerSystem
{
	void update(entt::registry& registry)
	{
		glm::vec3 v = { 0.f, 0.f, 0.f };
		glm::vec3 omega = { 0.f, 0.f, 0.f };

		if (g_InputAdmin.isKeyPressed(SDLK_w))
		{
			v.y = 5.f;
		}
		else if (g_InputAdmin.isKeyPressed(SDLK_s))
		{
			v.y = -5.f;
		}
		if (g_InputAdmin.isKeyPressed(SDLK_a))
		{
			v.x = -5.f;
		}
		else if (g_InputAdmin.isKeyPressed(SDLK_d))
		{
			v.x = 5.f;
		}
		if (g_InputAdmin.isKeyPressed(SDLK_q))
		{
			v.z = -3.f;
		}
		else if (g_InputAdmin.isKeyPressed(SDLK_e))
		{
			v.z = 3.f;
		}

		if (g_InputAdmin.isKeyPressed(SDLK_r))
		{
			omega.x = -3.f;
		}
		else if (g_InputAdmin.isKeyPressed(SDLK_f))
		{
			omega.x = 3.f;
		}
		
		if (g_InputAdmin.isKeyPressed(SDLK_t))
		{
			omega.y = -3.f;
		}
		else if (g_InputAdmin.isKeyPressed(SDLK_g))
		{
			omega.y = 3.f;
		}

		if (g_InputAdmin.isKeyPressed(SDLK_y))
		{
			omega.z = -3.f;
		}
		else if (g_InputAdmin.isKeyPressed(SDLK_h))
		{
			omega.z = 3.f;
		}

		const auto view = registry.view<Player, Kinetic>();
		for (const auto e : view)
		{
			auto& k = view.get<Kinetic>(e);
			k.velocity = v;
			k.omega = omega;
		}
	}
}