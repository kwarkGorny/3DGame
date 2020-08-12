#pragma once

#include "basic/Fseconds.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <entt/entity/fwd.hpp>
#include <sol/lua_table.hpp>

struct PlayerShipData
{
	explicit PlayerShipData(const sol::lua_table& table)
		: shootingFrequency(table["shootingFrequency"].get_or(1))
		, bulletsVelocity(table["bulletsVelocity"].get_or(1.f))
		, manoeuveringEnginesThrust(table["manoeuveringEnginesThrust"].get_or(1.f))
		, shipMass(table["shipMass"].get_or(1.f))
	{}
	Fseconds shootingFrequency = std::chrono::seconds(1);
	float bulletsVelocity = 1.f;
	float manoeuveringEnginesThrust = 1.f;
	float shipMass = 1.f;
};

struct AsteroidSpawnerData
{
	AsteroidSpawnerData(const sol::lua_table& table)
		: asteroidFrequency(table["asteroidFrequency"].get_or(1.f))
		, asteroidFrequencyIncrease(table["asteroidFrequencyIncrease"].get_or(1.f))
		, omegaRange(table["asteroidsAngularVelocityRange"][1].get_or(-glm::pi<float>()), table["asteroidsAngularVelocityRange"][2].get_or(glm::pi<float>()))
	{}
	float asteroidFrequency = 1;
	float asteroidFrequencyIncrease = 1;
	glm::vec2 omegaRange = { -glm::pi<float>(), glm::pi<float>() };
};

void createPlayer(entt::registry& registry, const PlayerShipData& shipData);

void createBullet(entt::registry& registry);

void createAsteroid(entt::registry& registry, int id, glm::vec2 position, glm::vec3 omega);

void createAsteroidSpawner(entt::registry& registry, const AsteroidSpawnerData& spawnerData);

void createBackground(entt::registry& registry, float playerForwardVelocity);