#pragma once

#include <functional>
#include <entt/entity/registry.hpp>

using OnClickCallback = std::function<void(entt::registry&, entt::entity)>;

struct OnClick
{
	OnClickCallback callback;
};