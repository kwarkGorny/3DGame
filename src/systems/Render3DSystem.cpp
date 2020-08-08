#include "Render3DSystem.hpp"

#include "components/Renderable3D.hpp"
#include "components/Transform.hpp"


#include "opengl/Renderer.hpp"

#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Render3DSystem
{
	void draw(entt::registry& registry)
	{
		glm::vec2 size = { 1920, 1080 };
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(size.x) / static_cast<float>(size.y), 0.1f, 100.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -3.0f));

		const auto entities = registry.view<Renderable3D, Transform>();
		for (const auto entity : entities)
		{
			const auto& t = entities.get<Transform>(entity).transform;
			const auto& renderable = entities.get<Renderable3D>(entity);
			renderable.texture->bind();
			renderable.shader->bind();
			renderable.shader->setUniform("u_MVP", projection * view * t);
			Renderer::draw(*renderable.vertexArray, *renderable.indexBuffer, *renderable.shader);
		}

	}
}