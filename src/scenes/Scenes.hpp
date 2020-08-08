#pragma once

#include "scenes/IScene.hpp"

#include <vector>
#include <memory>

class Scenes
{
public:
	void update(Fseconds dt);
	void draw();
	void handleEvent(const SDL_Event& event);

	void push_back(std::unique_ptr<IScene>&& scene) { m_Scenes.push_back(std::move(scene)); }
	void pop_back() { m_Scenes.pop_back(); }
	void clear() { m_Scenes.clear(); }

	[[nodiscard]] bool isValid()const noexcept { return !m_Scenes.empty(); }

private:
	std::vector<std::unique_ptr<IScene>> m_Scenes;
};