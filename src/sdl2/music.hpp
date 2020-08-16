#pragma once

#include <SDL_mixer.h>
#include <utility>
#include <string>

namespace sdl2
{
	class Music
	{
	public:
		constexpr Music()noexcept = default;
		Music(const std::string& file)noexcept : m_Music(Mix_LoadMUS(file.c_str())) {}

		~Music()noexcept
		{
			if (m_Music)
			{
				Mix_FreeMusic(m_Music);
			}
		}

		Music(Music&) = delete;
		Music(Music&& m) noexcept : m_Music(m.m_Music) { m.m_Music = nullptr; };

		Music& operator=(Music&) = delete;
		Music& operator=(Music&& m) noexcept { std::swap(m_Music, m.m_Music); return *this; };

		[[nodiscard]] bool isValid()const noexcept { return m_Music != nullptr; }

		[[nodiscard]] Mix_Music* get()const noexcept { return m_Music; }

	protected:
		Mix_Music* m_Music = nullptr;
	};
}
