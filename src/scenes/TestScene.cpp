#include "TestScene.hpp"

#include "basic/FileUtils.hpp"

#include "opengl/Renderer.hpp"

#include <array>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL_events.h>

constexpr std::array g_TestPositions = {
	-0.5f, -0.5f, 0.f, 0.f,
	0.5f, -0.5f, 1.f, 0.f,
	0.5f, 0.5f, 1.f, 1.f,
	-0.5f, 0.5f, 0.f, 1.f,
};

constexpr std::array<unsigned int, 6> g_TestIndices = {
	0, 1, 2,
	2, 3, 0
};

TestScene::TestScene()
	: m_VertexArray()
	, m_VertexBuffer(&g_TestPositions, g_TestPositions.size() * sizeof(float))
	, m_IndexBuffer(g_TestIndices.data(), g_TestIndices.size())
	, m_Texture("data/textures/button.png")
	, m_Shader(loadFile("data/shaders/basic.vs"), loadFile("data/shaders/basic.fs"))
{
	glm::vec2 size = { 640, 480 };
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(size.x) / static_cast<float>(size.y), 0.1f, 100.0f);//glm::ortho(0.f, static_cast<float>(g_ScreenSize.x), 0.f, static_cast<float>(g_ScreenSize.y));
	glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 mvp = projection * view * model;

	VertexLayout vertexLayout;
	vertexLayout.push<float>(2);
	vertexLayout.push<float>(2);
	m_VertexArray.add(m_VertexBuffer, vertexLayout);

	m_Texture.bind();
	m_Shader.bind();
	m_Shader.setUniform("u_Texture", 0);
	m_Shader.setUniform("u_MVP", mvp);

	m_VertexArray.unbind();
	m_VertexBuffer.unbind();
	m_IndexBuffer.unbind();
	//m_Texture.unbind();
	m_Shader.unbind();
}

bool TestScene::update(Fseconds dt) noexcept
{
	return false;
}

bool TestScene::draw() noexcept
{
	Renderer::draw(m_VertexArray, m_IndexBuffer, m_Shader);
	return false;
}

bool TestScene::handleEvent(const SDL_Event& event) noexcept
{
	return false;
}
