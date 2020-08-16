#include "DebugSystem.hpp"

#include "admins/InputAdmin.hpp"
#include "admins/ShaderAdmin.hpp"

#include "components/Transform.hpp"
#include "components/Colliders.hpp"
#include "components/Player.hpp"

#include "contexts/Camera.hpp"

#include "opengl/Renderer.hpp"

#include "basic/Logger.hpp"

#include <entt/entity/registry.hpp>

namespace DebugSystem
{
	void update(entt::registry& registry)
	{
		if (g_InputAdmin.isKeyPressed(SDLK_F4))
		{
			const auto mPos = g_InputAdmin.getMousePosition();
			logger::info("mouse pos x: {}, y: {}", mPos.x, mPos.y);
		}
		if (g_InputAdmin.isKeyPressed(SDLK_F5))
		{
			const auto players = registry.view<Player, Transform>();
			for (const auto player : players)
			{
				const auto& pos = players.get<Transform>(player).position;
				logger::info("player {} pos x: {}, y: {}, z: {}", player, pos.x, pos.y, pos.z);
			}
		}
	}

	void draw(entt::registry& registry)
	{
		const auto cameraPtr = registry.try_ctx<Camera>();
		if (!cameraPtr)
		{
			return;
		}
		const auto& camera = *cameraPtr;

		if (g_InputAdmin.isKeyPressed(SDLK_F6))
		{
			//glDisable(GL_DEPTH_TEST);

			const auto shader = g_ShaderAdmin["data/shaders/color"];
			shader->bind();
			shader->setUniform("u_Color", glm::vec4(1.f));
			shader->setUniform("u_View", camera.view);
			shader->setUniform("u_Projection", camera.projection);
			const auto view = registry.view<RectCollider, Transform>();
			for (const auto entity : view)
			{
				const auto& transform = view.get<Transform>(entity);
				const auto& rect = view.get<RectCollider>(entity);
				
				glm::mat4 model(1.f);
				model = glm::translate(model, glm::vec3(transform.position.x + rect.pos.x, transform.position.y + rect.pos.y, 0.f));
				model = glm::scale(model, glm::vec3(transform.scale.x * rect.size.x, transform.scale.y * rect.size.y, 1.f));

				shader->setUniform("u_Model", model);

				Renderer::drawQuad();
				
			}
			//glEnable(GL_DEPTH_TEST);
		}
	}
}