#pragma once

#include <functional>
#include <entt/entity/registry.hpp>

struct OnObsolete
{
	std::function<void(entt::registry&, entt::entity)> callback;
};