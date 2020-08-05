#include <iostream>

#include "sdl2/root.hpp"
#include "sdl2/window.hpp"
#include "sdl2/events.hpp"

int main(int argc, char* argv[]) 
{
	sdl2::Root sdl2Root(sdl2::SystemFlag::VIDEO | sdl2::SystemFlag::EVENTS);
	sdl2::Window window("Space Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, sdl2::WindowFlags::SHOWN);

	bool running = true;
	while (running)
	{
		sdl2::events::pollAll([&running](const SDL_Event& event) {
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
		});
	}

	return 0;
}