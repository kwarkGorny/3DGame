#pragma once

#include "basic/Fseconds.hpp"

union SDL_Event;

class IScene
{
public:
	virtual ~IScene() = default;
	virtual bool update(Fseconds dt)noexcept = 0;
	virtual bool draw()noexcept = 0;
	virtual bool handleEvent(const SDL_Event& event)noexcept = 0;
};