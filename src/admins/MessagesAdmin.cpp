#include "MessagesAdmin.hpp"

#include "scenes/Scenes.hpp"

void MessagesAdmin::update(Scenes& scenes)noexcept
{
	for (const auto& action : m_Actions)
	{
		action(scenes);
	}
	m_Actions.clear();
}

void MessagesAdmin::action(Message&& call)noexcept
{
	m_Actions.emplace_back(std::move(call));
}