#pragma once

#include "basic/FileUtils.hpp"

#include "opengl/IndexBuffer.hpp"
#include "opengl/VertexArray.hpp"
#include "opengl/VertexBuffer.hpp"
#include "opengl/Texture.hpp"
#include "opengl/Shader.hpp"

#include <memory>

struct Renderable3D
{
	std::unique_ptr<VertexArray> vertexArray = nullptr;
	std::unique_ptr<VertexBuffer> vertexBuffer = nullptr;
	std::unique_ptr<IndexBuffer> indexBuffer = nullptr;
	std::unique_ptr<Texture> texture = nullptr;
	std::unique_ptr<Shader> shader = nullptr;
};
