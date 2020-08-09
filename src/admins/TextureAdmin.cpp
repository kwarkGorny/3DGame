#include "TextureAdmin.hpp"

#include "basic/Logger.hpp"

SharedTexture TextureAdmin::operator[](const std::string& id) noexcept
{
	auto& weak = m_Resources[id];
	if (!weak.expired())
	{
		return weak.lock();
	}
	const auto resource = std::make_shared<Texture>(id);
	weak = resource;
	return resource;
}

void TextureAdmin::clear()noexcept
{ 
	m_Resources.clear();
}
