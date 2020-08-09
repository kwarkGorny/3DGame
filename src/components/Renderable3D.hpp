#pragma once

#include "basic/FileUtils.hpp"

#include "opengl/Mesh.hpp"
#include "opengl/Texture.hpp"
#include "opengl/Shader.hpp"
#include "opengl/Material.hpp"

#include <memory>

struct Renderable3D
{
	SharedMesh mesh = nullptr;
	SharedTexture texture = nullptr;
	SharedShader shader = nullptr;
	Material material = {};
};
