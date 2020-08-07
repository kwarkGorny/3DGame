#pragma once

#include "basic/Fseconds.hpp"
#include "sdl2/root.hpp"
#include "sdl2/window.hpp"

class Application
{
public:
	bool initialize(const std::string& title, int width, int height)noexcept;

	void run(Fseconds fps, Fseconds maxDelay, Fseconds slowWarring)noexcept;

private:
	sdl2::Root m_Root = sdl2::Root(sdl2::SystemFlag::VIDEO | sdl2::SystemFlag::EVENTS);
	sdl2::Window m_Window = sdl2::Window();
	bool m_IsOpen = false;
	bool m_IsPaused = false;
};
