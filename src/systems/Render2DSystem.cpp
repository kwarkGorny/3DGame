#include "Render2DSystem.hpp"

#include "admins/ShaderAdmin.hpp"

#include "components/Renderable2D.hpp"
#include "components/Transform.hpp"

#include "contexts/Camera.hpp"

#include "opengl/Renderer.hpp"

#include <entt/entity/registry.hpp>

namespace Render2DSystem
{
	void draw(entt::registry& registry)
	{
		const auto cameraPtr = registry.try_ctx<Camera>();
		if (!cameraPtr)
		{
			return;
		}
		const auto& camera = *cameraPtr;

		const auto& shader = g_ShaderAdmin["data/shaders/basic2d"];
		shader->bind();
		shader->setUniform("u_Texture", 0);
		shader->setUniform("u_View", camera.view);
		shader->setUniform("u_Projection", camera.projection);


		const auto entities = registry.view<Renderable2D, Transform>();
		for (const auto entity : entities)
		{
			const auto& t = entities.get<Transform>(entity);
			const auto& renderable = entities.get<Renderable2D>(entity);
			renderable.texture->bind();
			shader->setUniform("u_Color", renderable.color);
			glm::vec2 size = { renderable.texture->getWidth(), renderable.texture->getHeight() };
			shader->setUniform("u_Model", t.toModelMatrix(size));

			Renderer::drawQuad();
		}
	}
}