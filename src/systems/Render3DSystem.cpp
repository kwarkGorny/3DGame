#include "Render3DSystem.hpp"

#include "components/Renderable3D.hpp"
#include "components/Transform.hpp"

#include "contexts/Camera.hpp"
#include "contexts/SceneFrameBuffer.hpp"

#include "opengl/Renderer.hpp"
#include "opengl/Light.hpp"

#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Render3DSystem
{
	void draw(entt::registry& registry)
	{
		const auto cameraPtr = registry.try_ctx<Camera>();
		if (!cameraPtr)
		{
			return;
		}
		const auto& camera = *cameraPtr;
		const auto& sceneBuffer = registry.ctx<SceneFrameBuffer>();
		sceneBuffer.framebuffer->bind();
		Renderer::clear();

		Light light = {
			glm::vec3{1.f,-1.f, 1.f},
			glm::vec3{1.f, 1.f, 1.f}, //ambient
			glm::vec3{1.f, 1.f, 1.f}, //diffuse
			glm::vec3{1.0f, 1.0f, 1.0f}  //specular
		};


		const auto entities = registry.group<Renderable3D, Transform>();
		for (const auto entity : entities)
		{
			const auto& t = entities.get<Transform>(entity);
			const auto& renderable = entities.get<Renderable3D>(entity);
			const auto& shader = *renderable.shader;
			renderable.texture->bind();
			shader.bind();
			shader.setUniform("u_Material", renderable.material);
			shader.setUniform("u_Light", light);
			shader.setUniform("u_Model", t.toModelMatrix());
			shader.setUniform("u_View", camera.view);
			shader.setUniform("u_Projection", camera.projection);
			shader.setUniform("u_CameraPos", camera.position);
			
			Renderer::draw(*renderable.mesh);
		}
		sceneBuffer.framebuffer->unbind();
		sceneBuffer.shader->bind();

		sceneBuffer.framebuffer->bindTexture();
		glBindVertexArray(sceneBuffer.framebuffer->m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}