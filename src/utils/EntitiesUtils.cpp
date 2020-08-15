#include "EntitiesUtils.hpp"

#include "admins/MeshAdmin.hpp"
#include "admins/ShaderAdmin.hpp"
#include "admins/TextureAdmin.hpp"

#include "contexts/ResourcesCache.hpp"

#include "components/AsteroidSpawner.hpp"
#include "components/Asteroid.hpp"
#include "components/Bullet.hpp"
#include "components/Colliders.hpp"
#include "components/Dynamic.hpp"
#include "components/Kinetic.hpp"
#include "components/Particles.hpp"
#include "components/Player.hpp"
#include "components/Renderable3D.hpp"
#include "components/Bounded.hpp"
#include "components/Transform.hpp"
#include "components/Skybox.hpp"
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
            g_TextureAdmin["data/textures/particle.png"],
            g_TextureAdmin["data/obj/bullet/bullet.png"],
            g_TextureAdmin["data/obj/asteroid1/asteroid1.png"],
            g_TextureAdmin["data/obj/asteroid2/asteroid2.png"],
            g_TextureAdmin["data/obj/asteroid3/asteroid3.png"]
        },
        {
            g_ShaderAdmin["data/shaders/basic"],
            g_ShaderAdmin["data/shaders/particle"]
        }
    });
}

void createPlayer(entt::registry& registry, const PlayerShipData& shipData)
{
	const auto player = registry.create();
	registry.emplace<Transform>(player, glm::vec3( 0, -25.f, 0.f), glm::vec3(glm::pi<float>() / 2, glm::pi<float>(), 0), glm::vec3(1.f, 1.f, 1.f));
    registry.emplace<Kinetic>(player);
    registry.emplace<Dynamic>(player, Dynamic{ glm::vec3{}, shipData.shipMass });
    registry.emplace<Player>(player, shipData.manoeuveringEnginesThrust);
    registry.emplace<Weapon>(player, Weapon{ Timer{ shipData.shootingFrequency }, glm::vec3{0, 2, -0.5f}, shipData.bulletsVelocity });
    registry.emplace<RectCollider>(player);

	auto& renderable = registry.emplace<Renderable3D>(player);
	renderable.mesh = g_MeshAdmin["data/obj/ship/InfraredFurtive.obj"];
	renderable.texture = g_TextureAdmin["data/obj/ship/InfraredFurtive.png"];
	renderable.shader = g_ShaderAdmin["data/shaders/basic"];
	renderable.material = { {0.3f, 0.3f, 0.3f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, 32.0f };
}

void createBullet(entt::registry& registry, Transform transform, Kinetic kinetic)
{
    const auto bullet = registry.create();
    registry.emplace<Transform>(bullet, transform);
    registry.emplace<Kinetic>(bullet, kinetic);
    registry.emplace<Bullet>(bullet);
    registry.emplace<Bounded>(bullet);
    registry.emplace<RectCollider>(bullet);
    auto& renderable = registry.emplace<Renderable3D>(bullet);
    renderable.mesh = g_MeshAdmin["data/obj/bullet/bullet.obj"];
    renderable.texture = g_TextureAdmin["data/obj/bullet/bullet.png"];
    renderable.shader = g_ShaderAdmin["data/shaders/basic"];
    renderable.material = { {0.3f, 0.3f, 0.3f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, 32.0f };
}

void createAsteroid(entt::registry& registry, int id, glm::vec3 position, glm::vec3 omega)
{
    const auto asteroid = registry.create();
    registry.emplace<Transform>(asteroid, Transform{ glm::vec3(position.x, position.y, position.z) });
    registry.emplace<Kinetic>(asteroid, Kinetic{ {0, -5, 0}, omega });
    registry.emplace<Asteroid>(asteroid);
    registry.emplace<Bounded>(asteroid);
    registry.emplace<RectCollider>(asteroid);

    auto& renderable = registry.emplace<Renderable3D>(asteroid);
    const std::string asteroidId = "asteroid" + std::to_string(id);
    renderable.mesh = g_MeshAdmin[fmt::format("data/obj/{0}/{0}.obj", asteroidId)];
    renderable.texture = g_TextureAdmin[fmt::format("data/obj/{0}/{0}.png", asteroidId)];
    renderable.shader = g_ShaderAdmin["data/shaders/basic"];
    renderable.material = { {0.3f, 0.3f, 0.3f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, 32.0f };
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
    const std::array<std::string, 6> facesFilePaths{
        "data/textures/skybox/right.png",
        "data/textures/skybox/left.png",
        "data/textures/skybox/bottom.png",
        "data/textures/skybox/top.png",
        "data/textures/skybox/front.png",
        "data/textures/skybox/back.png"
    };
    logger::info("Background created placeholder");
    const auto bg = registry.create();
    registry.emplace<Transform>(bg);
    registry.emplace<Kinetic>(bg, Kinetic{ {0, 0, 0}, { -0.05f, 0.f, 0.f} });
    registry.emplace<Skybox>(bg,
        std::make_unique<Cubemap>(facesFilePaths),
        g_ShaderAdmin["data/shaders/skybox"]
    );
}

void createExplosion(entt::registry& registry, Transform transform)
{
    const auto explosion = registry.create();
    auto& emmitter = registry.emplace<ParticlesEmmiter>(explosion);
    emmitter.emitRate = 100;
    emmitter.generators.emplace_back(new BoxPosGen(glm::vec3{ 0.0, 0.0, 0.0}, glm::vec3{ 0.0, 0.0, 0.0 }));
    emmitter.generators.emplace_back(new BasicColorGen(
        glm::vec4{ 0.7, 0.7, 0.7, 1.0 },
        glm::vec4{ 1.0, 1.0, 1.0, 1.0 },
        glm::vec4{ 0.5, 0.0, 0.6, 0.0 },
        glm::vec4{ 0.7, 0.5, 1.0, 0.0 }
    ));
    emmitter.generators.emplace_back(new BasicVelGen(glm::vec3{ -0.05f, 0.22f, -0.05f}, glm::vec3{ 0.05f, 0.25f, 0.05f }));
    emmitter.generators.emplace_back(new BasicTimeGen(Fseconds(3.f), Fseconds(4.f)));

    auto& container = registry.emplace<ParticlesContainer>(explosion);
    container.initialize(
        g_ShaderAdmin["data/shaders/particle"],
        g_TextureAdmin["data/textures/particle.png"],
        500
     );
}