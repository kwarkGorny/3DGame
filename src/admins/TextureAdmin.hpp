#pragma once

#include "opengl/Texture.hpp"

#include <unordered_map>
#include <string>
#include <memory>

class TextureAdmin
{
public:
	[[nodiscard]] SharedTexture operator[](const std::string& id) noexcept;

	void clear()noexcept;
protected:
	std::unordered_map<std::string, std::weak_ptr<Texture>> m_Resources;
};

inline TextureAdmin g_TextureAdmin;