#include "InputAdmin.hpp"

#include <SDL_events.h>

void InputAdmin::handleEvent(const SDL_Event& event)noexcept
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		m_PressedKeys[event.key.keysym.sym] = true;
		break;
	case SDL_KEYUP:
		m_PressedKeys[event.key.keysym.sym] = false;
		break;
	case SDL_MOUSEBUTTONDOWN:
		m_MousePos.x = event.button.x;
		m_MousePos.y = event.button.y;
		m_MouseButtonDown = true;
		break;
	case SDL_MOUSEMOTION:
		m_MousePos.x = event.motion.x;
		m_MousePos.y = event.motion.y;
		break;
	case SDL_MOUSEBUTTONUP:
		m_MouseButtonDown = false;
		break;
	default:
		break;
	}
}

bool InputAdmin::isKeyPressed(SDL_Keycode code)const noexcept 
{
	const auto itr = m_PressedKeys.find(code);
	return itr != m_PressedKeys.cend() && itr->second;
}