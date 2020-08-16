#pragma once

#include "opengl/Mesh.hpp"
#include "opengl/Shader.hpp"
#include "opengl/Texture.hpp"

#include "sdl2/sound.hpp"

#include <vector>

struct ResourcesCache
{
	std::vector<SharedMesh> meshes;
	std::vector<SharedTexture> textures;
	std::vector<SharedShader> shaders;
	std::vector<SharedSound> sounds;
};