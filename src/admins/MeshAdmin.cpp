#include "MeshAdmin.hpp"

#include "basic/Logger.hpp"

SharedMesh MeshAdmin::operator[](const std::string& objFilePath) noexcept
{
	auto& weak = m_Resources[objFilePath];
	if (!weak.expired())
	{
		return weak.lock();
	}
	const std::string mtlDir(objFilePath.begin(), objFilePath.begin() + objFilePath.find_last_of('/'));
	const MeshCPU cpuMesh{ objFilePath, mtlDir };
	const auto resource = std::make_shared<Mesh>(cpuMesh);
	weak = resource;
	return resource;
}

void MeshAdmin::clear()noexcept
{ 
	m_Resources.clear();
}
