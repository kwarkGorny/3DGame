#pragma once

#include "scenes/IScene.hpp"
#include "opengl/Mesh.hpp"

#include <entt/entity/registry.hpp>

class TestScene : public IScene
{
public:
	TestScene();
	virtual ~TestScene() = default;

	bool update(Fseconds dt)noexcept final;
	bool draw()noexcept final;
	bool handleEvent(const SDL_Event& event)noexcept final;

private:
	entt::registry m_Registry;
};