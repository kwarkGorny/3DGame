#pragma once

#include "opengl/Shader.hpp"

#include <unordered_map>
#include <string>

class ShaderAdmin
{
public:
	[[nodiscard]] SharedShader operator[](const std::string& id) noexcept;

	void clear()noexcept;

protected:
	std::unordered_map<std::string, std::weak_ptr<Shader>> m_Resources;
};

inline ShaderAdmin g_ShaderAdmin;