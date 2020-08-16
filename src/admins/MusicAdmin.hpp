#pragma once

#include "sdl2/music.hpp"

#include <string>

class MusicAdmin
{
public:
	void initialize();

	void clear();

	bool play(const std::string& musicId, int loops = -1);
	bool play(const sdl2::Music& music, int loops = -1)const;

	[[nodiscard]] float getVolume()const { return m_Volume; }
	void setVolume(float volume);
	[[nodiscard]] bool isPlaying()const;

protected:
	sdl2::Music m_CurrentMusic;
	std::string m_CurrentMusicId;
	float m_Volume = 1.f;
};

inline MusicAdmin g_MusicAdmin;
