#pragma once

#include "basic/Fseconds.hpp"

#include "components/Kinetic.hpp"
#include "components/Transform.hpp"
#include "components/Asteroid.hpp"
#include "components/OnClick.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <entt/entity/fwd.hpp>
#include <sol/lua_table.hpp>

struct PlayerShipData
{
	explicit PlayerShipData(const sol::lua_table& table)
		: shootingFrequency()
		, bulletsVelocity(table["canonBulletsVelocity"].get_or(1.f))
		, manoeuveringEnginesThrust(table["manoeuveringEnginesThrust"].get_or(1.f))
		, shipMass(table["spaceShipMass"].get_or(1.f))
	{
		const auto frequency = table["canonShootingFrequency"].get_or(1.f);
		shootingFrequency = Fseconds(frequency > 0 ? (1.f / frequency) : 1.f);
	}
	Fseconds shootingFrequency = std::chrono::seconds(1);
	float bulletsVelocity = 1.f;
	float manoeuveringEnginesThrust = 1.f;
	float shipMass = 1.f;
};

struct AsteroidSpawnerData
{
	AsteroidSpawnerData(const sol::lua_table& table)
		: asteroidFrequency(table["initialAsteroidAppearanceFrequency"].get_or(1.f))
		, asteroidFrequencyIncrease(table["asteroidAppearanceFrequencyIncrease"].get_or(1.f))
		, omegaRange(table["asteroidsAngularVelocityRange"][1].get_or(-glm::pi<float>()), table["asteroidsAngularVelocityRange"][2].get_or(glm::pi<float>()))
		, prefab{ Fseconds(table["explosionDuration"].get_or(0.1f)) }
	{}
	float asteroidFrequency = 1;
	float asteroidFrequencyIncrease = 1;
	glm::vec2 omegaRange = { -glm::pi<float>(), glm::pi<float>() };
	Asteroid prefab;
};

void createResourcesCache(entt::registry& registry);

void createPlayer(entt::registry& registry, const PlayerShipData& shipData);

void createBullet(entt::registry& registry, glm::vec3 position, Kinetic kinetic);

void createAsteroid(entt::registry& registry, int id, glm::vec3 position, glm::vec3 omega, Asteroid prefab);

void createAsteroidSpawner(entt::registry& registry, const AsteroidSpawnerData& spawnerData);

void createBackground(entt::registry& registry, float playerForwardVelocity);

void createPlayerExplosion(entt::registry& registry, glm::vec3 position);

void createAsteroidExplosion(entt::registry& registry, glm::vec3 position, Fseconds explosionDuration);

void createBulletExplosion(entt::registry& registry, glm::vec3 position);

void createSprite(entt::registry& registry, glm::vec3 position, glm::vec3 scale, const std::string& textureId);

void createButton(entt::registry& registry, glm::vec3 position, glm::vec3 scale, const std::string& textureId, OnClickCallback&& onClick);