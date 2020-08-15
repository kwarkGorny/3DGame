#pragma once

#include "opengl/Cubemap.hpp"
#include "opengl/Shader.hpp"

struct Skybox
{
	UniqueCubemap cubemap = nullptr;
	SharedShader shader = nullptr;
};
