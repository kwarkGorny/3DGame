#pragma once

#include "scenes/IScene.hpp"
#include "opengl/Mesh.hpp"

#include <entt/entity/registry.hpp>

class GameScene : public IScene
{
public:
	GameScene();
	virtual ~GameScene() = default;

	bool update(Fseconds dt)noexcept final;
	bool draw()noexcept final;
	bool handleEvent(const SDL_Event& event)noexcept final;

private:
	entt::registry m_Registry;
};