#pragma once

#include "basic/Timer.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct RectCollider
{
	glm::vec3 pos;
	glm::vec3 size;
};

struct Rect2DCollider
{
	glm::vec2 pos;
	glm::vec2 size;

	[[nodiscard]] constexpr bool intersects(float x, float y)const noexcept
	{
		return x > pos.x  &&
			x < pos.x + size.x &&
			y > pos.y&&
			y < pos.y + size.y;
	}
};