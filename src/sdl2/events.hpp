#pragma once

#include <SDL_events.h>
#include <vector>
#include <chrono>
#include <memory>

namespace sdl2
{
	enum class EventPushResult : int
	{
		SUCCESS = 1,
		FILTERED = 0,
		FAIL = -1,
	};

	enum class EventState : int
	{
		QUERY = -1,
		DISABLE = 0,
		ENABLE = 1
	};

	using EventFilter = SDL_EventFilter;
	using EventWatch = SDL_EventFilter;

	namespace events
	{
		inline int requestQuit()
		{
			SDL_PumpEvents();
			return SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT);
		}

		inline void pump() { SDL_PumpEvents(); }

		inline int add(std::vector<SDL_Event>& events, std::uint32_t minType, std::uint32_t maxType) { return SDL_PeepEvents(events.data(), events.size(), SDL_ADDEVENT, minType, maxType); }

		inline int peek(std::vector<SDL_Event>& events, std::uint32_t minType, std::uint32_t maxType) { return SDL_PeepEvents(events.data(), events.size(), SDL_PEEKEVENT, minType, maxType); }

		inline int get(std::vector<SDL_Event>& events, std::uint32_t minType, std::uint32_t maxType) { return SDL_PeepEvents(events.data(), events.size(), SDL_GETEVENT, minType, maxType); }

		[[nodiscard]] inline bool has() { return SDL_PollEvent(nullptr) == 1; }

		[[nodiscard]] inline bool has(std::uint32_t type) { return SDL_HasEvent(type) == SDL_TRUE; }

		[[nodiscard]] inline bool has(std::uint32_t minType, std::uint32_t maxType) { return SDL_HasEvents(minType, maxType) == SDL_TRUE; }

		inline void flush(std::uint32_t type) { SDL_FlushEvent(type); }

		inline void flush(std::uint32_t minType, std::uint32_t maxType) { SDL_FlushEvents(minType, maxType); }

		template<class OnEvent, class... Args>
		void pollAll(OnEvent&& onEvent, Args&&... args)
		{
			SDL_Event event;
			while (sdl2::events::poll(event))
			{
				onEvent(event, args...);
			}
		}

		inline bool poll(SDL_Event& event) { return SDL_PollEvent(&event) == 1; }

		inline bool wait(SDL_Event& event) { return SDL_WaitEvent(&event) == 1; }

		inline bool wait(SDL_Event& event, std::chrono::milliseconds timeout) { return SDL_WaitEventTimeout(&event, timeout.count()) == 1; }

		inline EventPushResult push(SDL_Event* event) { return static_cast<EventPushResult>(SDL_PushEvent(event)); }

		inline void set(sdl2::EventFilter filter, void* userData) { SDL_SetEventFilter(filter, userData); }

		[[nodiscard]] inline std::pair<sdl2::EventFilter, void*> get()
		{
			std::pair<sdl2::EventFilter, void*> p;
			SDL_GetEventFilter(&p.first, &p.second);
			return p;
		}

		inline void add(sdl2::EventWatch filter, void* userData) { SDL_AddEventWatch(filter, userData); }

		inline void remove(sdl2::EventWatch filter, void* userData) { SDL_DelEventWatch(filter, userData); }

		inline void filter(sdl2::EventFilter filter, void** userData) { SDL_FilterEvents(filter, userData); }

		inline void setState(std::uint32_t type, EventState state) { SDL_EventState(type, static_cast<int>(state)); }

		inline void disable(std::uint32_t type) { setState(type, EventState::DISABLE); }

		inline void enable(std::uint32_t type) { setState(type, EventState::ENABLE); }

		[[nodiscard]] inline std::uint8_t query(std::uint32_t type) { return SDL_EventState(type, static_cast<int>(EventState::QUERY)); }

		inline std::uint32_t registerEvents(int numevents) { return SDL_RegisterEvents(numevents); }
	}
}
