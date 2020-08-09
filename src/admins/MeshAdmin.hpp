#pragma once

#include "opengl/Mesh.hpp"

#include <unordered_map>
#include <string>
#include <memory>

class MeshAdmin
{
public:
	[[nodiscard]] SharedMesh operator[](const std::string& id) noexcept;

	void clear()noexcept;
protected:
	std::unordered_map<std::string, std::weak_ptr<Mesh>> m_Resources;
};

inline MeshAdmin g_MeshAdmin;