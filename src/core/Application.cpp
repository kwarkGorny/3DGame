#include "Application.hpp"

#include "basic/Logger.hpp"
#include "opengl/Renderer.hpp"
#include "sdl2/events.hpp"

#include <thread>

#include <gl/glew.h>
#include <SDL_opengl.h>

#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <array>
#include <cassert>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "opengl/VertexBuffer.hpp"
#include "opengl/IndexBuffer.hpp"
#include "opengl/VertexArray.hpp"
#include "opengl/Shader.hpp"
#include "opengl/Texture.hpp"

std::string loadFile(const std::string& filePath)
{
	std::ifstream t(filePath);
	if (!t.is_open())
	{
		return "";
	}
	t.seekg(0, std::ios::end);
	const auto size = t.tellg();
	t.seekg(0);
	std::string file(size, ' ');
	t.read(&file[0], size);
	return file;
}


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

	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		logger::error("Warning: Unable to set VSync! SDL Error: {}", SDL_GetError());
		return false;
	}
	m_IsOpen = true;
	CHECK_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	CHECK_GL(glEnable(GL_BLEND));
	//glDebugMessageCallback(debugCallback, nullptr);

	return true;
}

void Application::run(Fseconds fps, Fseconds maxDelay, Fseconds slowWarring)noexcept
{
	if (!m_IsOpen)
	{
		return;
	}

	constexpr std::array positions = {
		-0.5f, -0.5f, 0.f, 0.f,
		0.5f, -0.5f, 1.f, 0.f,
		0.5f, 0.5f, 1.f, 1.f,
		-0.5f, 0.5f, 0.f, 1.f,
	};

	constexpr std::array<unsigned int, 6> indices = {
		0, 1, 2,
		2, 3, 0
	};

	auto size = m_Window.getSize();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(size.x) / static_cast<float>(size.y), 0.1f, 100.0f);//glm::ortho(0.f, static_cast<float>(g_ScreenSize.x), 0.f, static_cast<float>(g_ScreenSize.y));
	glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 mvp = projection * view * model;

	VertexArray vertexArray;

	VertexBuffer vertexBuffer(&positions, positions.size() * sizeof(float));

	VertexLayout vertexLayout;
	vertexLayout.push<float>(2);
	vertexLayout.push<float>(2);
	vertexArray.add(vertexBuffer, vertexLayout);

	IndexBuffer indexBuffer(indices.data(), indices.size());

	Texture texture("data/textures/button.png");
	texture.bind();

	Shader shader(loadFile("data/shaders/basic.vs"), loadFile("data/shaders/basic.fs"));
	shader.bind();
	//shader.setUniform("u_Color", glm::vec4(0.8f, 0.5f, 0.5f, 1.f));
	shader.setUniform("u_Texture", 0);
	shader.setUniform("u_MVP", mvp);

	vertexArray.unbind();
	shader.unbind();
	vertexBuffer.unbind();
	indexBuffer.unbind();
	//

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
			if (event.type == SDL_QUIT)
			{
				m_IsOpen = false;
			}
			else if (event.type == SDL_WINDOWEVENT)
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
		});

		if (!m_IsPaused)
		{
			//for (int frameCount = static_cast<int>(dt / g_TimePerFrame); frameCount != 0; --frameCount)
			//{
				//update(g_TimePerFrame);
			//}

			Renderer::clear();

			Renderer::draw(vertexArray, indexBuffer, shader);

			m_Window.glSwap();
		}

		//m_IsOpen &= m_Scenes.isValid();

		const auto dtFrame = std::chrono::duration_cast<Fseconds>(std::chrono::steady_clock::now() - now);
		logger::warning(slowWarring > dtFrame, "Slow Frame dt {}s", dtFrame.count());

		std::this_thread::sleep_for(fps - dtFrame);
	}

}
