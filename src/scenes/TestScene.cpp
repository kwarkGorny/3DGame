#include "TestScene.hpp"

#include "contexts/SceneSize.hpp"


#include "systems/AsteroidSpawerSystem.hpp"
#include "systems/DynamicSystem.hpp"
#include "systems/KineticSystem.hpp"
#include "systems/ObsoleteSystem.hpp"
#include "systems/PlayerSystem.hpp"
#include "systems/Render3DSystem.hpp"
#include "systems/WeaponSystem.hpp"

#include "utils/EntitiesUtils.hpp"

#include <array>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL_events.h>
#include <sol/sol.hpp>
#include <sol/lua_table.hpp>

TestScene::TestScene()
{
	sol::state luaState;
	luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
	const auto levelConfig = luaState.script_file("data/lua/config.lua").get<sol::lua_table>();
	
	m_Registry.set<SceneSize>(glm::ivec2(1920, 1080));
	createPlayer(m_Registry, {});
	//createBullet(m_Registry);
	//createAsteroidSpawner(m_Registry, {});
}

bool TestScene::update(Fseconds dt) noexcept
{
	PlayerSystem::update(m_Registry);
	DynamicSystem::update(m_Registry, dt);
	KineticSystem::update(m_Registry, dt);

	AsteroidSpawnerSystem::update(m_Registry, dt);
	WeaponSystem::update(m_Registry, dt);

	ObsoleteSystem::update(m_Registry);
	return false;
}

bool TestScene::draw() noexcept
{
	float fov = glm::radians(60.f);
	float nearPlane = 0.3f;
	float farPlane = 1000.f;
	glm::ivec2 size = { 1920, 1080 };
	//glm::mat4 projection = glm::ortho(0, size.x, 0, size.y);
	glm::mat4 projection = glm::perspective(fov, static_cast<float>(size.x) / static_cast<float>(size.y), nearPlane, farPlane);

	glm::vec3 camPos(0.f, 0.f, 10.f);
	//glm::mat4 view = glm::mat4(1.f);
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
