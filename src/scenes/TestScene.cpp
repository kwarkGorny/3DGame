#include "TestScene.hpp"

#include "components/Kinetic.hpp"
#include "components/Renderable3D.hpp"
#include "components/Transform.hpp"

#include "systems/KineticSystem.hpp"
#include "systems/Render3DSystem.hpp"

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
{
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	const auto test = m_Registry.create();
	m_Registry.emplace<Transform>(test, glm::mat4(1.0f));
	m_Registry.emplace<Kinetic>(test, glm::vec3(0.f, 0.f, -0.5f));

	auto& renderable = m_Registry.emplace<Renderable3D>(test);
	renderable.vertexArray = std::make_unique<VertexArray>();
	renderable.vertexBuffer = std::make_unique<VertexBuffer>(g_TestPositions.data(), g_TestPositions.size() * sizeof(float));
	renderable.indexBuffer = std::make_unique<IndexBuffer>(g_TestIndices.data(), g_TestIndices.size());
	renderable.texture = std::make_unique<Texture>("data/textures/button.png");
	renderable.shader = std::make_unique<Shader>(loadFile("data/shaders/basic.vs"), loadFile("data/shaders/basic.fs"));

	VertexLayout vertexLayout;
	vertexLayout.push<float>(2);
	vertexLayout.push<float>(2);
	renderable.vertexArray->add(*renderable.vertexBuffer, vertexLayout);

	renderable.texture->bind();
	renderable.shader->bind();
	renderable.shader->setUniform("u_Texture", 0);
	renderable.shader->setUniform("u_MVP", model);

	renderable.vertexArray->unbind();
	renderable.vertexBuffer->unbind();
	renderable.indexBuffer->unbind();
	renderable.shader->unbind();
}

bool TestScene::update(Fseconds dt) noexcept
{
	KineticSystem::update(m_Registry, dt);
	return false;
}

bool TestScene::draw() noexcept
{
	Render3DSystem::draw(m_Registry);
	return false;
}

bool TestScene::handleEvent(const SDL_Event& event) noexcept
{
	return false;
}
