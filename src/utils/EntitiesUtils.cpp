#include "EntitiesUtils.hpp"

#include "admins/MeshAdmin.hpp"
#include "admins/ShaderAdmin.hpp"
#include "admins/TextureAdmin.hpp"

#include "contexts/ResourcesCache.hpp"

#include "components/AsteroidSpawner.hpp"
#include "components/Asteroid.hpp"
#include "components/Bullet.hpp"
#include "components/Dynamic.hpp"
#include "components/Kinetic.hpp"
#include "components/Player.hpp"
#include "components/Renderable3D.hpp"
#include "components/Bounded.hpp"
#include "components/Transform.hpp"
#include "components/Weapon.hpp"

#include "opengl/Mesh.hpp"

#include "basic/Logger.hpp"

#include <entt/entity/registry.hpp>

void createResourcesCache(entt::registry& registry)
{
    registry.set<ResourcesCache>(ResourcesCache{
        {
            g_MeshAdmin["data/obj/bullet/bullet.obj"],
            g_MeshAdmin["data/obj/asteroid1/asteroid1.obj"],
            g_MeshAdmin["data/obj/asteroid2/asteroid2.obj"],
            g_MeshAdmin["data/obj/asteroid3/asteroid3.obj"]
        },
        {
            g_TextureAdmin["data/obj/bullet/bullet.png"],
            g_TextureAdmin["data/obj/asteroid1/asteroid1.png"],
            g_TextureAdmin["data/obj/asteroid2/asteroid2.png"],
            g_TextureAdmin["data/obj/asteroid3/asteroid3.png"]
        },
        {
            g_ShaderAdmin["data/shaders/basic"]
        }
    });
}

void createPlayer(entt::registry& registry, const PlayerShipData& shipData)
{
	const auto test = registry.create();
	registry.emplace<Transform>(test, glm::vec3( 0, 0, 0.f), glm::vec3(glm::pi<float>() / 2, glm::pi<float>(), 0), glm::vec3(1.f, 1.f, 1.f));
    registry.emplace<Kinetic>(test);
    registry.emplace<Dynamic>(test, Dynamic{ glm::vec3{}, shipData.shipMass });
    registry.emplace<Player>(test, shipData.manoeuveringEnginesThrust);
    registry.emplace<Weapon>(test, Weapon{ Timer{ shipData.shootingFrequency }, glm::vec3{0, 2, -0.5f}, shipData.bulletsVelocity });

	auto& renderable = registry.emplace<Renderable3D>(test);
	renderable.mesh = g_MeshAdmin["data/obj/ship/InfraredFurtive.obj"];
	renderable.texture = g_TextureAdmin["data/obj/ship/InfraredFurtive.png"];
	renderable.shader = g_ShaderAdmin["data/shaders/basic"];
	renderable.material = { {0.1f, 0.1f, 0.1f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, 32.0f };
}

void createBullet(entt::registry& registry, Transform transform, Kinetic kinetic)
{
    const auto bullet = registry.create();
    registry.emplace<Transform>(bullet, transform);
    registry.emplace<Kinetic>(bullet, kinetic);
    registry.emplace<Bullet>(bullet);
    registry.emplace<Bounded>(bullet);
    auto& renderable = registry.emplace<Renderable3D>(bullet);
    renderable.mesh = g_MeshAdmin["data/obj/bullet/bullet.obj"];
    renderable.texture = g_TextureAdmin["data/obj/bullet/bullet.png"];
    renderable.shader = g_ShaderAdmin["data/shaders/basic"];
    renderable.material = { {1.0f, 0.5f, 0.31f}, {1.0f, 0.5f, 0.31f}, {0.5f, 0.5f, 0.5f}, 32.0f };
}

void createAsteroid(entt::registry& registry, int id, glm::vec3 position, glm::vec3 omega)
{
    const auto asteroid = registry.create();
    registry.emplace<Transform>(asteroid, Transform{ glm::vec3(position.x, position.y, position.z) });
    registry.emplace<Kinetic>(asteroid, Kinetic{ {0, -5, 0}, omega });
    registry.emplace<Asteroid>(asteroid);
    registry.emplace<Bounded>(asteroid);
    auto& renderable = registry.emplace<Renderable3D>(asteroid);
    const std::string asteroidId = "asteroid" + std::to_string(id);
    renderable.mesh = g_MeshAdmin[fmt::format("data/obj/{0}/{0}.obj", asteroidId)];
    renderable.texture = g_TextureAdmin[fmt::format("data/obj/{0}/{0}.png", asteroidId)];
    renderable.shader = g_ShaderAdmin["data/shaders/basic"];
    renderable.material = { {1.0f, 0.5f, 0.31f}, {1.0f, 0.5f, 0.31f}, {0.5f, 0.5f, 0.5f}, 32.0f };
}

void createAsteroidSpawner(entt::registry& registry, const AsteroidSpawnerData& spawnerData)
{
    const auto test = registry.create();
    registry.emplace<AsteroidSpawner>(test, AsteroidSpawner{ 
        Timer{ Fseconds(spawnerData.asteroidFrequency)},
        spawnerData.asteroidFrequency,
        spawnerData.asteroidFrequencyIncrease,
        spawnerData.omegaRange
    });
}

void createBackground(entt::registry& registry, float playerForwardVelocity)
{
    logger::info("Background created placeholder");
}

void createExplosion(entt::registry& registry, Transform transform)
{

}