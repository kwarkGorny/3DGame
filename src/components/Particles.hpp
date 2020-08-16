#pragma once
#include "basic/Fseconds.hpp"

#include "opengl/Shader.hpp"
#include "opengl/Texture.hpp"

#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/random.hpp>
#include <glm/common.hpp>

struct ParticleTimer
{
	Fseconds dt = Fseconds::zero();
	Fseconds duration = Fseconds::zero();
	float progress = 0.f;
};

struct ParticlesContainer
{
	void initialize(const SharedShader& s, std::size_t maxSize);
	void deinitialize();
	void kill(std::size_t id);
	void wake(std::size_t id);
	void copyData(std::size_t fromId, std::size_t toId);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> velocities;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec4> startColors;
	std::vector<glm::vec4> endColors;
	std::vector<ParticleTimer> timers;
	std::vector<bool> alives;

	std::size_t count{ 0 };
	std::size_t countAlive{ 0 };

	unsigned int vao;
	unsigned int positionBuffer;
	unsigned int colorBuffer;
	SharedShader shader;

	bool initialized = false;

	using PositionType = typename std::decay<decltype(*positions.begin())>::type;
	using ColorType = typename std::decay<decltype(*colors.begin())>::type;
};

class ParticlesGenerator
{
public:
	virtual ~ParticlesGenerator() = default;
	virtual void generate(Fseconds dt, ParticlesContainer& p, size_t startId, size_t endId) = 0;
};

class PositionGenerator : public ParticlesGenerator
{
public:
	PositionGenerator(glm::vec3 position, glm::vec3 offset)
		: position{position}
		, offset{offset}
	{};
	void generate(Fseconds dt, ParticlesContainer& p, size_t startId, size_t endId) final;
	glm::vec3 position;
	glm::vec3 offset;
};

class VelocityGenerator : public ParticlesGenerator
{
public:
	VelocityGenerator(glm::vec3 minVel, glm::vec3 maxVel)
		: minStartVel{ minVel }
		, maxStartVel{ maxVel }
	{}
	void generate(Fseconds dt, ParticlesContainer& p, size_t startId, size_t endId) final;
	glm::vec3 minStartVel;
	glm::vec3 maxStartVel;
};

class TimeGenerator : public ParticlesGenerator
{
public:
	TimeGenerator(Fseconds minT, Fseconds maxT)
		: minTime{minT}
		, maxTime{maxT}
	{};
	void generate(Fseconds dt, ParticlesContainer& p, size_t startId, size_t endId) final;
	Fseconds minTime;
	Fseconds maxTime;
};

class ColorGenerator : public ParticlesGenerator
{
public:
	ColorGenerator(glm::vec4 minStartCol, glm::vec4 maxStartCol,	glm::vec4 minEndCol, glm::vec4 maxEndCol)
		: minStartCol(minStartCol)
		, maxStartCol(maxStartCol)
		, minEndCol(minEndCol)
		, maxEndCol(maxEndCol)
	{}

	void generate(Fseconds dt, ParticlesContainer& p, size_t startId, size_t endId) final;

	glm::vec4 minStartCol;
	glm::vec4 maxStartCol;
	glm::vec4 minEndCol;
	glm::vec4 maxEndCol;
};

struct ParticlesEmmiter
{
	Fseconds duration;
	float emitRate = 0.0f;
	std::vector<std::unique_ptr<ParticlesGenerator>> generators = {};
	bool infinite = false;
};
