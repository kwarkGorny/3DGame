#pragma once

#include "scenes/IScene.hpp"

#include <entt/entity/registry.hpp>

class RetryScene : public IScene
{
public:
	RetryScene();
	virtual ~RetryScene() = default;

	bool update(Fseconds dt)noexcept final;
	bool draw()noexcept final;
	bool handleEvent(const SDL_Event& event)noexcept final;

private:
	entt::registry m_Registry;
};