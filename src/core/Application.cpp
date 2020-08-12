#include "Application.hpp"

#include "admins/InputAdmin.hpp"
#include "basic/Logger.hpp"
#include "opengl/GLDebug.hpp"
#include "opengl/Renderer.hpp"
#include "scenes/TestScene.hpp"
#include "sdl2/events.hpp"

#include <thread>

#include <gl/glew.h>
#include <SDL_opengl.h>

bool Application::initialize(const std::string& title, int width, int height)noexcept
{
	m_Window = sdl2::Window(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, sdl2::WindowFlags::OPENGL | sdl2::WindowFlags::SHOWN);
	if (!m_Window.isValid())
	{
		return false;
	}

	if (const auto context = m_Window.createGLContext();
		context == nullptr)
	{
		logger::error("Error initializing gl context! {}", SDL_GetError());
		return false;
	}

	glewExperimental = GL_TRUE;
	if (const auto glewError = glewInit();
		glewError != GLEW_OK)
	{
		logger::error("Error initializing GLEW! {}", glewGetErrorString(glewError));
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		logger::error("Warning: Unable to set VSync! SDL Error: {}", SDL_GetError());
		return false;
	}
	m_IsOpen = true;
	Renderer::initialize();
	m_Scenes.push_back(std::make_unique<TestScene>());
	return true;
}

void Application::run(Fseconds fps, Fseconds maxDelay, Fseconds slowWarring)noexcept
{
	if (!m_IsOpen)
	{
		return;
	}
	auto last = std::chrono::steady_clock::now();
	while (m_IsOpen)
	{
		const auto now = std::chrono::steady_clock::now();
		const auto dt = std::chrono::duration_cast<Fseconds>(now - last);
		if (dt < fps)
		{
			continue;
		}
		last = now;
		if (dt > maxDelay)
		{
			logger::info("Big delta time skipping it {}s", dt.count());
			std::this_thread::sleep_for(fps);
			continue;
		}

		sdl2::events::pollAll([this](const SDL_Event& event) {
			m_IsOpen &= event.type != SDL_QUIT;
			if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
				{
					m_IsPaused = true;
					logger::info("Game paused due to focuse lost");
				}
				else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
				{
					m_IsPaused = false;
					logger::info("Game unpaused due to focuse gain");
				}
			}
			g_InputAdmin.handleEvent(event);
			m_Scenes.handleEvent(event);
		});

		if (!m_IsPaused)
		{
			for (int frameCount = static_cast<int>(dt / fps); frameCount != 0; --frameCount)
			{
				m_Scenes.update(fps);
			}

			Renderer::clear();

			m_Scenes.draw();

			m_Window.glSwap();

			Renderer::flush();
		}

		m_IsOpen &= m_Scenes.isValid();

		const auto dtFrame = std::chrono::duration_cast<Fseconds>(std::chrono::steady_clock::now() - now);
		logger::warning(dtFrame > slowWarring, "Slow Frame dt {}s", dtFrame.count());

		std::this_thread::sleep_for(fps - dtFrame);
	}
}
