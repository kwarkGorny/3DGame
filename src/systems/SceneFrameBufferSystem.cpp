#include "SceneFrameBufferSystem.hpp"

#include "contexts/SceneFrameBuffer.hpp"

#include "opengl/Renderer.hpp"

#include <entt/entity/registry.hpp>

namespace SceneFrameBufferSystem
{
	void predraw(entt::registry& registry)
	{
		const auto sceneBufferPtr = registry.try_ctx<SceneFrameBuffer>();
		if (!sceneBufferPtr)
		{
			return;
		}
		sceneBufferPtr->framebuffer->bind();
		Renderer::clear();
	}

	void postdraw(entt::registry& registry)
	{
		const auto sceneBufferPtr = registry.try_ctx<SceneFrameBuffer>();
		if (!sceneBufferPtr)
		{
			return;
		}
		const auto& sceneBuffer = *sceneBufferPtr;
		sceneBuffer.framebuffer->unbind();
		sceneBuffer.shader->bind();
		sceneBuffer.framebuffer->bindTexture();
		glDisable(GL_DEPTH_TEST);
		Renderer::drawQuad();
		glEnable(GL_DEPTH_TEST);
	}
}