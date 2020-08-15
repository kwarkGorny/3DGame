#include "Particles.hpp"

#include "basic/Fseconds.hpp"

#include "opengl/Shader.hpp"

#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/random.hpp>
#include <glm/ext/scalar_constants.hpp>

#include "opengl/GLDebug.hpp"

void ParticlesContainer::initialize(const SharedShader& s,const SharedTexture& t, size_t maxSize)
{
	if (initialized)
	{
		return;
	}
	initialized = true;
	shader = s;
	texture = t;
	count = maxSize;
	countAlive = 0;

	positions.resize(maxSize);
	colors.resize(maxSize);
	startColors.resize(maxSize);
	endColors.resize(maxSize);
	velocities.resize(maxSize);
	timers.resize(maxSize);
	alives.resize(maxSize, false);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PositionType) * count, nullptr, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PositionType), nullptr);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ColorType) * count, nullptr, GL_STREAM_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ColorType), nullptr);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticlesContainer::deinitialize()
{
	if (!initialized)
	{
		return;
	}
	initialized = false;
	shader = nullptr;
	texture = nullptr;
	count = 0;
	countAlive = 0;

	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteVertexArrays(1, &vao);
}


void ParticlesContainer::kill(size_t id)
{
	if (countAlive > 0)
	{
		copyData(countAlive - 1, id);
		alives[id] = false;
		--countAlive;
	}
}

void ParticlesContainer::wake(size_t id)
{
	if (countAlive < count)
	{
		copyData(countAlive, id);
		alives[id] = true;
		++countAlive;
	}
}

void ParticlesContainer::copyData(std::size_t fromId, std::size_t toId)
{
	positions[toId] = positions[fromId];
	colors[toId] = colors[fromId];
	startColors[toId] = startColors[fromId];
	endColors[toId] = endColors[fromId];
	velocities[toId] = velocities[fromId];
	timers[toId] = timers[fromId];
}

void BoxPosGen::generate(Fseconds dt, ParticlesContainer& p, size_t startId, size_t endId)
{
	glm::vec4 posMin{ position.x - offset.x, position.y - offset.y, position.z - offset.z, 1.0 };
	glm::vec4 posMax{ position.x + offset.x, position.y + offset.y, position.z + offset.z, 1.0 };

	for (size_t i = startId; i < endId; ++i)
	{
		p.positions[i] = glm::linearRand(posMin, posMax);
	}
}

void RoundPositionGenerator::generate(Fseconds dt, ParticlesContainer& p, size_t startId, size_t endId) 
{
	for (std::size_t i = startId; i < endId; ++i)
	{
		const double ang = glm::linearRand(0.0, glm::pi<double>() * 2.0);
		p.positions[i] = position + glm::vec3(radius * std::sin(ang), radius * std::cos(ang), 0.0);
	}
}

void BasicVelGen::generate(Fseconds dt, ParticlesContainer& p, size_t startId, size_t endId) 
{
	for (size_t i = startId; i < endId; ++i)
	{
		p.velocities[i] = glm::linearRand(minStartVel, maxStartVel);
	}
}

void BasicTimeGen::generate(Fseconds dt, ParticlesContainer& p, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		p.timers[i].duration = Fseconds(glm::linearRand(minTime.count(), maxTime.count()));
	}
}

void BasicColorGen::generate(Fseconds dt, ParticlesContainer& p, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		p.startColors[i] = glm::linearRand(minStartCol, maxStartCol);
		p.endColors[i] = glm::linearRand(minEndCol, maxEndCol);
	}
}