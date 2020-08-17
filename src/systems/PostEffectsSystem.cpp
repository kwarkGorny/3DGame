#include "OnClickSystem.hpp"

#include "admins/ShaderAdmin.hpp"

#include "contexts/SceneFrameBuffer.hpp"

#include <entt/entity/registry.hpp>
#include <SDL_events.h>

namespace PostEffectsSystem
{
	void handleEvent(entt::registry& registry, const SDL_Event& event)
	{
		if (event.type == SDL_KEYUP)
		{
			auto sceneBufferPtr = registry.try_ctx<SceneFrameBuffer>();
			if (!sceneBufferPtr)
			{
				return;
			}
			if (event.key.keysym.sym == SDLK_F1)
			{
				sceneBufferPtr->shader = g_ShaderAdmin["data/shaders/framebufferShader"];
			}
			else if (event.key.keysym.sym == SDLK_F2)
			{
				sceneBufferPtr->shader = g_ShaderAdmin["data/shaders/postEffects/greyScale"];
			}
			else if(event.key.keysym.sym == SDLK_F3)
			{
				sceneBufferPtr->shader = g_ShaderAdmin["data/shaders/postEffects/inversion"];
			}

		}
	}
}
