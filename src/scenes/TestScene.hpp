#pragma once

#include "scenes/IScene.hpp"

#include "opengl/IndexBuffer.hpp"
#include "opengl/VertexArray.hpp"
#include "opengl/VertexBuffer.hpp"
#include "opengl/Texture.hpp"
#include "opengl/Shader.hpp"

class TestScene : public IScene
{
public:
	TestScene();
	virtual ~TestScene() = default;

	bool update(Fseconds dt)noexcept final;
	bool draw()noexcept final;
	bool handleEvent(const SDL_Event& event)noexcept final;

private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	Texture m_Texture;
	Shader m_Shader;
};