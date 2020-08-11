#include "ObsoleteSystem.hpp"

#include "components/Obsolete.hpp"

#include <entt/entity/registry.hpp>

namespace ObsoleteSystem
{
	void update(entt::registry& registry)
	{
		const auto view = registry.view<Obsolete>();
		registry.destroy(view.begin(), view.end());
	}
}