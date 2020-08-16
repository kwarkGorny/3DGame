#include "SkyboxSystem.hpp"

#include "components/Skybox.hpp"
#include "components/Transform.hpp"

#include "contexts/Camera.hpp"

#include "opengl/Renderer.hpp"

#include <entt/entity/registry.hpp>

namespace SkyboxSystem
{
	void draw(entt::registry& registry)
	{
		const auto cameraPtr = registry.try_ctx<Camera>();
		if (!cameraPtr)
		{
			return;
		}
		glDisable(GL_DEPTH_TEST);

		const auto& camera = *cameraPtr;
		const glm::mat4 view = glm::mat4(glm::mat3(camera.view));

		const auto entities = registry.view<Skybox, Transform>();
		for (const auto entity : entities)
		{
			const auto& t = entities.get<Transform>(entity);
			const auto& skybox = entities.get<Skybox>(entity);
			skybox.cubemap->bind();
			const auto& shader = *skybox.shader;
			shader.bind();
			shader.setUniform("u_Model", t.toModelMatrix());
			shader.setUniform("u_View", view);
			shader.setUniform("u_Projection", camera.projection);

			glDrawArrays(GL_TRIANGLES, 0, 36);
			skybox.cubemap->unbind();
		}
		glEnable(GL_DEPTH_TEST);
	}
}