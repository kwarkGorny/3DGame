#pragma once

#include <SDL_mixer.h>
#include <utility>
#include <string>
#include <memory>

namespace sdl2
{
	class Sound
	{
	public:
		constexpr Sound()noexcept = default;
		Sound(const std::string& file)noexcept
			: m_Sound(Mix_LoadWAV(file.c_str()))
		{}

		~Sound()noexcept
		{
			if (m_Sound)
			{
				Mix_FreeChunk(m_Sound);
			}
		}

		Sound(Sound&) = delete;
		Sound(Sound&& s) noexcept : m_Sound(s.m_Sound) { s.m_Sound = nullptr; };

		Sound& operator=(Sound&) = delete;
		Sound& operator=(Sound&& s) noexcept { std::swap(m_Sound, s.m_Sound); return *this; };

		[[nodiscard]] bool isValid()const noexcept { return m_Sound != nullptr; }

		[[nodiscard]] Mix_Chunk* get()const noexcept { return m_Sound; }

	protected:
		Mix_Chunk* m_Sound = nullptr;

	};
}
using SharedSound = std::shared_ptr<sdl2::Sound>;

