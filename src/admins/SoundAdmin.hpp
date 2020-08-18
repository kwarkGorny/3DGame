#pragma once

#include "sdl2/sound.hpp"

#include <unordered_map>
#include <string>

class SoundAdmin
{
public:
	void initialize()noexcept;

	[[nodiscard]] SharedSound operator[](const std::string& id) noexcept;

	void clear()noexcept;

	int play(const std::string& soundId, int loops = 0, int channel = -1)noexcept;
	int play(const SharedSound& sound, int loops = 0, int channel = -1)noexcept;
	[[nodiscard]] bool hasVolume()const noexcept { return m_Volume > 0.f; }
	[[nodiscard]] float getVolume()const noexcept { return m_Volume; }
	void setVolume(float volume)noexcept;

	void erase(int channel) { m_SoundPerChannel.erase(channel); }
protected:
	std::unordered_map<std::string, std::weak_ptr<sdl2::Sound>> m_Resources;
	std::unordered_map<int, SharedSound> m_SoundPerChannel;
	float m_Volume = 1.f;
};

inline SoundAdmin g_SoundAdmin;
