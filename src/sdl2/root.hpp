#pragma once

#include <type_traits>
#include <cstdint>
#include <SDL.h>

namespace sdl2
{
	enum class SystemFlag : std::uint32_t
	{
		TIMER = SDL_INIT_TIMER,
		AUDIO = SDL_INIT_AUDIO,
		VIDEO = SDL_INIT_VIDEO,
		JOYSTICK = SDL_INIT_JOYSTICK,
		HAPTIC = SDL_INIT_HAPTIC,
		GAMECONTROLLER = SDL_INIT_GAMECONTROLLER,
		EVENTS = SDL_INIT_EVENTS,
		SENSOR = SDL_INIT_SENSOR,
		NOPARACHUTE = SDL_INIT_NOPARACHUTE,
		EVERYTHING = SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER | SDL_INIT_SENSOR
	};

	constexpr SystemFlag operator|(SystemFlag a, SystemFlag b)noexcept
	{
		return static_cast<SystemFlag>(static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b));
	}

	inline bool init(SystemFlag flags) noexcept
	{
		return SDL_Init(static_cast<std::uint32_t>(flags)) >= 0;
	}

	inline void quit() noexcept { SDL_Quit(); }

	class [[nodiscard]] Root
	{
	public:
		Root(SystemFlag flags)noexcept
		{
			const auto success = sdl2::init(flags);
			SDL_assert(success);
		}

		~Root()noexcept { sdl2::quit(); }
	};
}