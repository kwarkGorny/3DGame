#include "ShaderAdmin.hpp"

#include "basic/Logger.hpp"
#include "basic/FileUtils.hpp"

SharedShader ShaderAdmin::operator[](const std::string& id) noexcept
{
	auto& weak = m_Resources[id];
	if (!weak.expired())
	{
		return weak.lock();
	}
	const auto resource = std::make_shared<Shader>(loadFile(id + ".vs"), loadFile(id + ".fs"));
	weak = resource;
	return resource;
}

void ShaderAdmin::clear()noexcept
{ 
	m_Resources.clear();
}
