#include "Scenes.hpp"

#include <SDL_events.h>

void Scenes::update(Fseconds dt)
{
	for (auto itr = m_Scenes.rbegin(); itr != m_Scenes.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
		{
			break;
		}
	}
}

void Scenes::draw()
{
	for (auto& scene : m_Scenes)
	{
		if (!scene->draw())
		{
			break;
		}
	}
}

void Scenes::handleEvent(const SDL_Event& event)
{
	for (auto itr = m_Scenes.rbegin(); itr != m_Scenes.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
		{
			break;
		}
	}
}