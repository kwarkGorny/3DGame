#pragma once

#include <entt/entity/fwd.hpp>

#include <glm/mat4x4.hpp>

struct RenderData
{
	glm::mat4 projection = glm::mat4(1.f);
	glm::mat4 view = glm::mat4(1.f);
};

namespace Render3DSystem
{
	void draw(entt::registry& registry, const RenderData& renderData);
}