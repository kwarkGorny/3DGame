#include "TestScene.hpp"

#include "admins/ShaderAdmin.hpp"

#include "contexts/Camera.hpp"
#include "contexts/ResourcesCache.hpp"
#include "contexts/SceneBound.hpp"
#include "contexts/SceneFrameBuffer.hpp"

#include "systems/AsteroidSpawerSystem.hpp"
#include "systems/CollisionSystem.hpp"
#include "systems/DebugSystem.hpp"
#include "systems/DynamicSystem.hpp"
#include "systems/KineticSystem.hpp"
#include "systems/ObsoleteSystem.hpp"
#include "systems/PlayerSystem.hpp"
#include "systems/Render3DSystem.hpp"
#include "systems/SceneBoundSystem.hpp"
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
	float fov = glm::radians(60.f);
	float nearPlane = 0.3f;
	float farPlane = 1000.f;
	glm::ivec2 size = { 1920, 1080 };
	glm::vec3 camPos( 0.f, 0.f, 50 );

	m_Registry.set<Camera>(
		camPos,
		glm::perspective(fov, static_cast<float>(size.x) / static_cast<float>(size.y), nearPlane, farPlane),
		glm::lookAt(camPos, camPos + glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f))
	);

	auto frameBufferShader = g_ShaderAdmin["data/shaders/framebufferShader"];
	frameBufferShader->bind();
	frameBufferShader->setUniform("u_Texture", 0);
	m_Registry.set<SceneBound>(glm::vec2(-50, -50), glm::vec2(50, 50));
	m_Registry.set<SceneFrameBuffer>(
		std::make_unique<FrameBuffer>(1920, 1080),
		frameBufferShader
	);

	sol::state luaState;
	luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
	const auto levelConfig = luaState.script_file("data/lua/config.lua").get<sol::lua_table>();
	const float shipforwardVelocity = levelConfig["spaceShipForwardVelocity"].get_or(1.0f);
	
	createResourcesCache(m_Registry);
	createPlayer(m_Registry, PlayerShipData{ levelConfig });
	//createAsteroidSpawner(m_Registry, AsteroidSpawnerData{ levelConfig });
	//createBackground(m_Registry, shipforwardVelocity);
}

bool TestScene::update(Fseconds dt) noexcept
{
	PlayerSystem::update(m_Registry);
	DynamicSystem::update(m_Registry, dt);
	KineticSystem::update(m_Registry, dt);

	AsteroidSpawnerSystem::update(m_Registry, dt);
	WeaponSystem::update(m_Registry, dt);

	SceneBoundSystem::update(m_Registry);
	CollisionSystem::update(m_Registry);

	ObsoleteSystem::update(m_Registry);

#ifdef DEVELOPMENT
	DebugSystem::update(m_Registry);
#endif // DEVELOPMENT

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
