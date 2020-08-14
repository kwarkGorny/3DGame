#pragma once

#include "opengl/Framebuffer.hpp"
#include "opengl/Shader.hpp"

struct SceneFrameBuffer
{
	UniqueFrameBuffer framebuffer;
	SharedShader shader;
};