#include "SoundAdmin.hpp"

#include "basic/Logger.hpp"
#include <utility>
void SoundAdmin::initialize()noexcept
{
	Mix_ChannelFinished([](auto channel) { g_SoundAdmin.erase(channel); });
}

SharedSound SoundAdmin::operator[](const std::string& id) noexcept
{
	auto& weak = m_Resources[id];
	if (!weak.expired())
	{
		return weak.lock();
	}
	const auto resource = std::make_shared<sdl2::Sound>(id);
	logger::warning(!resource->isValid(), "Trying to access resource which dont exist: {}", id);
	weak = resource;
	return resource;
}

void SoundAdmin::clear()noexcept
{
	m_Resources.clear();
}

int SoundAdmin::play(const std::string& soundId, int loops, int channel)noexcept
{
	return play((*this)[soundId], loops, channel);
}

int SoundAdmin::play(const SharedSound& sound, int loops, int channel)noexcept
{
	const int channelUsed = Mix_PlayChannel(channel, sound->get(), loops);
	m_SoundPerChannel[channelUsed] = sound;
	return channelUsed;
}

void SoundAdmin::setVolume(float volume)noexcept
{
	m_Volume = std::clamp(volume, 0.f, 1.f);
	Mix_Volume(-1, static_cast<int>(MIX_MAX_VOLUME * m_Volume));
}