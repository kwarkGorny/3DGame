#pragma once

#include <vector>
#include <functional>
#include <sol/forward.hpp>

class Scenes;
using Message = std::function<void(Scenes&)>;

class MessagesAdmin
{
public:
	void update(Scenes& scenes)noexcept;

	void action(Message&& call)noexcept;
private:
	std::vector<Message> m_Actions;
};

inline MessagesAdmin g_MessagesAdmin;
