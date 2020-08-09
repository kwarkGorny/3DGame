#pragma once

#include <SDL_keycode.h>
#include <unordered_map>
#include <glm/vec2.hpp>

union SDL_Event;

class InputAdmin
{
public:
	void handleEvent(const SDL_Event& event)noexcept;

	[[nodiscard]] bool isKeyPressed(SDL_Keycode code)const noexcept;

	[[nodiscard]] bool isMouseButtonDown()const noexcept { return m_MouseButtonDown; }

	[[nodiscard]] glm::ivec2 getMousePosition()const noexcept;

	[[nodiscard]] glm::vec2 getMousePositionF()const noexcept;

private:
	std::unordered_map<SDL_Keycode, bool> m_PressedKeys = {};
	bool m_MouseButtonDown = false;
	glm::ivec2 m_MousePos = {};
};

inline InputAdmin g_InputAdmin;