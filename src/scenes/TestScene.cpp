#include "TestScene.hpp"

#include "systems/AsteroidSpawerSystem.hpp"
#include "systems/DynamicSystem.hpp"
#include "systems/KineticSystem.hpp"
#include "systems/ObsoleteSystem.hpp"
#include "systems/PlayerSystem.hpp"
#include "systems/Render3DSystem.hpp"

#include "utils/EntitiesUtils.hpp"

#include <array>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL_events.h>

TestScene::TestScene()
{
	createPlayer(m_Registry);
	createAsteroidSpawner(m_Registry);
}

bool TestScene::update(Fseconds dt) noexcept
{
	PlayerSystem::update(m_Registry);
	DynamicSystem::update(m_Registry, dt);
	KineticSystem::update(m_Registry, dt);

	AsteroidSpawnerSystem::update(m_Registry, dt);

	ObsoleteSystem::update(m_Registry);
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
