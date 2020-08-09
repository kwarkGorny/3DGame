#include "Render3DSystem.hpp"

#include "components/Renderable3D.hpp"
#include "components/Transform.hpp"

#include "opengl/Renderer.hpp"

#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Render3DSystem
{
	void draw(entt::registry& registry, const RenderData& renderData)
	{
		Light light = {
			glm::vec3{0.f, 0.f, 5.f},
			glm::vec3{0.2f, 0.2f, 0.2f},//ambient
			glm::vec3{0.5f, 0.5f, 0.5f},//diffuse
			glm::vec3{1.0f, 1.0f, 1.0f}//specular
		};
		glm::vec3 camPos(0.f, 0.f, 5.f);

		const auto entities = registry.view<Renderable3D, Transform>();
		for (const auto entity : entities)
		{
			const auto& t = entities.get<Transform>(entity);
			const auto& renderable = entities.get<Renderable3D>(entity);
			renderable.texture->bind();
			const auto& shader = *renderable.shader;
			shader.bind();
			shader.setUniform("u_Texture", 0);
			//shader.setUniform("u_Material", renderable.material);
			//shader.setUniform("u_Light", light);
			shader.setUniform("u_Model", t.toModelView());
			shader.setUniform("u_View", renderData.view);
			shader.setUniform("u_Projection", renderData.projection);
			//shader.setUniform("u_CameraPos", camPos);
			
			Renderer::draw(*renderable.mesh);
		}
	}
}