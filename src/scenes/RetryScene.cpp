#include "RetryScene.hpp"

#include "admins/MusicAdmin.hpp"
#include "admins/ShaderAdmin.hpp"

#include "contexts/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <SDL_events.h>

RetryScene::RetryScene()
{
	glm::ivec2 size = { 1920, 1080 };
	glm::vec3 camPos(0.f, 0.f, 0);

	m_Registry.set<Camera>(
		camPos,
		glm::ortho(0.f, static_cast<float>(size.x), 0.f, static_cast<float>(size.y)),
		glm::lookAt(camPos, camPos + glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f))
	);

	g_MusicAdmin.play("data/musics/defeated.ogg");
}

bool RetryScene::update(Fseconds dt) noexcept
{
	return false;
}

bool RetryScene::draw() noexcept
{
	return true;
}

bool RetryScene::handleEvent(const SDL_Event& event) noexcept
{
	return false;
}
