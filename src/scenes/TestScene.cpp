#include "TestScene.hpp"

#include "admins/MeshAdmin.hpp"
#include "admins/ShaderAdmin.hpp"
#include "admins/TextureAdmin.hpp"

#include "components/Kinetic.hpp"
#include "components/Player.hpp"
#include "components/Renderable3D.hpp"
#include "components/Transform.hpp"

#include "systems/KineticSystem.hpp"
#include "systems/PlayerSystem.hpp"
#include "systems/Render3DSystem.hpp"

#include "opengl/Mesh.hpp"

#include <array>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL_events.h>

std::vector<Vertex> vertices = {
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f) },
        { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f) },
        { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f) },
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f) },

        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f) },
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f) },
        { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f) },
        { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f) },

        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f) },
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f) },

        { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f) },
        { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f) },
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f) },
        { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f) },

        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f) },
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f) },
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f) },

        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f) },
        { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f) },
        { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f) },
        { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f) },
        { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f) }
};
std::vector<unsigned int> indices = {};
/*
std::vector<Vertex> vertices = {
	{ glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f, 1.f) },
	{ glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f, 0.f) },
	{ glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f, 0.f) },
	{ glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f, 0.f) }
};

std::vector<unsigned int> indices = {
	0, 1, 2,
	0, 2, 3
};
*/

TestScene::TestScene()
{
	const auto test = m_Registry.create();
	m_Registry.emplace<Transform>(test);
	m_Registry.emplace<Kinetic>(test);
	m_Registry.emplace<Player>(test);

	auto& renderable = m_Registry.emplace<Renderable3D>(test);
	//renderable.mesh = std::make_shared<Mesh>(MeshCPU(vertices, indices));
    renderable.mesh = g_MeshAdmin["data/obj/ship/ship.obj"];
    renderable.texture = g_TextureAdmin["data/textures/teapot.png"];
	renderable.shader = g_ShaderAdmin["data/shaders/basic"];
    renderable.material = { {1.0f, 0.5f, 0.31f}, {1.0f, 0.5f, 0.31f}, {0.5f, 0.5f, 0.5f}, 32.0f };
}

bool TestScene::update(Fseconds dt) noexcept
{
	PlayerSystem::update(m_Registry);
	KineticSystem::update(m_Registry, dt);
	return false;
}

bool TestScene::draw() noexcept
{
	float fov = glm::radians(90.f);
	float nearPlane = 0.1f;
	float farPlane = 1000.f;
	glm::vec2 size = { 1920, 1080 };
	glm::mat4 projection = glm::perspective(fov, static_cast<float>(size.x) / static_cast<float>(size.y), nearPlane, farPlane);

	glm::vec3 camPos(0.f, 0.f, 5.f);
	glm::mat4 view = glm::lookAt(camPos, camPos + glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	RenderData data = {
		projection,
		view
	};

	Render3DSystem::draw(m_Registry, data);
	return false;
}

bool TestScene::handleEvent(const SDL_Event& event) noexcept
{
	return false;
}
