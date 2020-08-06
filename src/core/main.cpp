#include <gl/glew.h>

#include <SDL_opengl.h>
#include <glm/vec2.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <array>
#include <cassert>

#include "sdl2/root.hpp"
#include "sdl2/window.hpp"
#include "sdl2/events.hpp"

#include "opengl/Renderer.hpp"
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

void debugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam)
{
	std::cout << "[OPENGL ERROR]" << "source: " << source << " type: " << type << " id: " << id << " severity: " << severity << " message: " << std::string_view(message, length) << '\n';
}
int main(int argc, char* argv[]) 
{
	const sdl2::Root root(sdl2::SystemFlag::VIDEO | sdl2::SystemFlag::EVENTS);

	sdl2::Window window("Opengl test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, sdl2::WindowFlags::OPENGL | sdl2::WindowFlags::SHOWN);

	auto context = window.createGLContext();
	glewExperimental = GL_TRUE;
	const auto glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}

	glDebugMessageCallback(debugCallback, nullptr);

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

	CHECK_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	CHECK_GL(glEnable(GL_BLEND));

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

	vertexArray.unbind();
	shader.unbind();
	vertexBuffer.unbind();
	indexBuffer.unbind();

	Renderer renderer;

	bool running = true;
	while (running)
	{
		sdl2::events::pollAll([&running](const SDL_Event& event) {
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			});
		renderer.clear();

		shader.bind();

		renderer.draw(vertexArray, indexBuffer, shader);

		window.glSwap();
	}

	return 0;
}