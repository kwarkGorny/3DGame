#include "Scenes.hpp"

#include <SDL_events.h>

void Scenes::update(Fseconds dt)
{
	for (auto& scene : m_Scenes)
	{
		if (!scene->update(dt))
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
	for (auto& scene : m_Scenes)
	{
		if (!scene->handleEvent(event))
		{
			break;
		}
	}
}