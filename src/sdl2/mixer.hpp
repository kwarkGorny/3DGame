#pragma once

#include <cstdint>
#include <SDL_mixer.h>
#include <SDL_assert.h>

namespace sdl2
{
	enum class MixerFlag : int
	{
		FLAC = MIX_INIT_FLAC,
		MOD = MIX_INIT_MOD,
		MP3 = MIX_INIT_MP3,
		OGG = MIX_INIT_OGG,
		MID = MIX_INIT_MID,
		OPUS = MIX_INIT_OPUS,
		EVERYTHING = MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS
	};

	constexpr inline MixerFlag operator|(MixerFlag a, MixerFlag b)noexcept
	{
		return static_cast<MixerFlag>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline bool openMixerAudio(int frequency, std::uint16_t format, int channels, int chunkSize) { return Mix_OpenAudio(frequency, format, channels, chunkSize) == 0; }

	inline bool initMixer(MixerFlag flags) noexcept
	{
		return Mix_Init(static_cast<int>(flags)) >= 0;
	}

	inline void quitMixer() noexcept { Mix_Quit(); }

	class [[nodiscard]] MixerRoot
	{
	public:
		MixerRoot(MixerFlag flags)noexcept
		{
			const auto success = sdl2::initMixer(flags);
			SDL_assert(success);
		}

		~MixerRoot()noexcept { sdl2::quitMixer(); }
	};
}
