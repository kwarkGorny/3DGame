#include "MusicAdmin.hpp"

#include "basic/Logger.hpp"
#include <utility>

namespace 
{
	void onMusicStopped()
	{
		g_MusicAdmin.clear();
	}
}

void MusicAdmin::clear()
{
	m_CurrentMusicId = "";
	m_CurrentMusic = sdl2::Music();
}

void MusicAdmin::initialize()
{
	Mix_HookMusicFinished(onMusicStopped);
}

bool MusicAdmin::play(const std::string& musicId, int loops) 
{
	m_CurrentMusicId = musicId;
	m_CurrentMusic = sdl2::Music(musicId);
	logger::warning(!m_CurrentMusic.isValid(), "Trying to access resource which dont exist: {}", musicId);
	return play(m_CurrentMusic, loops);
}

bool MusicAdmin::play(const sdl2::Music& music, int loops)const
{
	return Mix_PlayMusic(music.get(), loops) != -1;
}

void MusicAdmin::setVolume(float volume)
{
	m_Volume = std::clamp(volume, 0.f, 1.f);
	Mix_VolumeMusic(static_cast<int>(MIX_MAX_VOLUME * m_Volume));
}

bool MusicAdmin::isPlaying()const
{
	return Mix_PlayingMusic() == 0;
}