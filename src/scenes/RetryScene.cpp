#include "RetryScene.hpp"

#include "admins/MessagesAdmin.hpp"
#include "admins/MusicAdmin.hpp"
#include "admins/ShaderAdmin.hpp"

#include "basic/Logger.hpp"

#include "contexts/Camera.hpp"
#include "contexts/ResourcesCache.hpp"

#include "scenes/Scenes.hpp"
#include "scenes/GameScene.hpp"

#include "systems/OnClickSystem.hpp"
#include "systems/Render2DSystem.hpp"

#include "utils/EntitiesUtils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <SDL_events.h>

RetryScene::RetryScene()
{
	glm::ivec2 size = { 1920, 1080 };
	glm::vec3 camPos(0.f, 0.f, 0.f);

	m_Registry.set<Camera>(
		camPos,
		glm::ortho(0.f, static_cast<float>(size.x), 0.f, static_cast<float>(size.y), -1.f, 1.f),
		glm::mat4(1.f)//glm::lookAt(camPos, camPos + glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f))
	);

	createSprite(m_Registry, glm::vec3(size.x / 2.f, size.y / 2.f + 155, 1.f), glm::vec3(0.25f), "data/textures/gui/header.png");
	createButton(m_Registry, glm::vec3(size.x / 2.f - 100, size.y / 2.f, 1.f), glm::vec3(0.25f), "data/textures/gui/replay_btn.png", [](auto& r, auto e) {
		g_MessagesAdmin.action([](Scenes& scenes) {
			scenes.clear();
			scenes.push_back(std::make_unique<GameScene>());
		});
	});
	createButton(m_Registry, glm::vec3(size.x / 2.f + 100, size.y / 2.f, 1.f), glm::vec3(0.25f), "data/textures/gui/close_btn.png", [](auto& r, auto e) {
		g_MessagesAdmin.action([](Scenes& scenes) { scenes.clear(); });
	});

	m_Registry.set<ResourcesCache>(ResourcesCache{ {}, {}, {g_ShaderAdmin["data/shaders/basic2d"]}, {} });

	g_MusicAdmin.play("data/musics/defeated.ogg");
}

bool RetryScene::update(Fseconds dt) noexcept
{
	return false;
}

bool RetryScene::draw() noexcept
{
	Render2DSystem::draw(m_Registry);
	return true;
}

bool RetryScene::handleEvent(const SDL_Event& event) noexcept
{
	OnClickSystem::handleEvent(m_Registry, event);
	return false;
}
