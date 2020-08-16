#include "EntitiesUtils.hpp"

#include "admins/MeshAdmin.hpp"
#include "admins/ShaderAdmin.hpp"
#include "admins/SoundAdmin.hpp"
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
#include "components/TimedObsolete.hpp"
#include "components/Transform.hpp"
#include "components/Skybox.hpp"
#include "components/Weapon.hpp"

#include "opengl/Mesh.hpp"

#include "basic/Logger.hpp"

#include <entt/entity/registry.hpp>
#include <utility>

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
            g_ShaderAdmin["data/shaders/particle"],
            g_ShaderAdmin["data/shaders/color"]
        },
        {
            g_SoundAdmin["data/sounds/explosion.wav"],
            g_SoundAdmin["data/sounds/shoot.wav"]
        }
    });
}

void createPlayer(entt::registry& registry, const PlayerShipData& shipData)
{
	const auto player = registry.create();
	registry.emplace<Transform>(player, glm::vec3( 0, -25.f, 0.f));
    registry.emplace<Kinetic>(player);
    registry.emplace<Dynamic>(player, Dynamic{ glm::vec3{}, shipData.shipMass });
    registry.emplace<Player>(player, shipData.manoeuveringEnginesThrust);
    registry.emplace<Weapon>(player, Weapon{ Timer{ shipData.shootingFrequency }, glm::vec3{0, 2, 0.f}, shipData.bulletsVelocity });

	auto& renderable = registry.emplace<Renderable3D>(player);
	renderable.mesh = g_MeshAdmin["data/obj/ship/Transtellar.obj"];
	renderable.texture = g_TextureAdmin["data/obj/ship/Transtellar.png"];
	renderable.shader = g_ShaderAdmin["data/shaders/basic"];
	renderable.material = { {0.7f, 0.7f, 0.7f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, 32.0f };

    const auto size = renderable.mesh->getSize();
    registry.emplace<RectCollider>(player, RectCollider{ -size / 2.f, size });

    auto& emmitter = registry.emplace<ParticlesEmmiter>(player);
    emmitter.infinite = true;
    emmitter.emitRate = 100;
    emmitter.generators.emplace_back(new PositionGenerator(glm::vec3{ 0, -1, 0.f }, glm::vec3{ 0, 0, 0 }));
    emmitter.generators.emplace_back(new ColorGenerator(
        glm::vec4{ 1.f, 1.f, 0.7f, 1.0 },
        glm::vec4{ 1.f, 0.75f, 0.44f, 1.0 },
        glm::vec4{ 1.f, 0.35f, 0.078f, 0.0 },
        glm::vec4{ 1.f, 0.137f, 0.137f, 0.0 }
    ));
    emmitter.generators.emplace_back(new VelocityGenerator(glm::vec3{ -1.f, -0.5f, 0.f }, glm::vec3{ 1.f, -2.f, 0.f }));
    emmitter.generators.emplace_back(new TimeGenerator(Fseconds(0.75f), Fseconds(1.f)));

    auto& container = registry.emplace<ParticlesContainer>(player);
    container.initialize(g_ShaderAdmin["data/shaders/particle"], 250);
}

void createBullet(entt::registry& registry, glm::vec3 position, Kinetic kinetic)
{
    const auto bullet = registry.create();
    registry.emplace<Transform>(bullet, position);
    registry.emplace<Kinetic>(bullet, kinetic);
    registry.emplace<Bullet>(bullet);
    registry.emplace<Bounded>(bullet);
    auto& renderable = registry.emplace<Renderable3D>(bullet);
    renderable.mesh = g_MeshAdmin["data/obj/bullet/bullet.obj"];
    renderable.texture = g_TextureAdmin["data/obj/bullet/bullet.png"];
    renderable.shader = g_ShaderAdmin["data/shaders/basic"];
    renderable.material = { {0.3f, 0.3f, 0.3f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, 32.0f };

    const auto size = renderable.mesh->getSize();
    registry.emplace<RectCollider>(bullet, RectCollider{ -size / 2.f, size });
}

void createAsteroid(entt::registry& registry, int id, glm::vec3 position, glm::vec3 omega, Asteroid prefab)
{
    const auto asteroid = registry.create();
    registry.emplace<Transform>(asteroid, glm::vec3(position.x, position.y, position.z));
    registry.emplace<Kinetic>(asteroid, Kinetic{ {0, -5, 0}, omega });
    registry.emplace<Asteroid>(asteroid, prefab);
    registry.emplace<Bounded>(asteroid);

    auto& renderable = registry.emplace<Renderable3D>(asteroid);
    const std::string asteroidId = "asteroid" + std::to_string(id);
    renderable.mesh = g_MeshAdmin[fmt::format("data/obj/{0}/{0}.obj", asteroidId)];
    renderable.texture = g_TextureAdmin[fmt::format("data/obj/{0}/{0}.png", asteroidId)];
    renderable.shader = g_ShaderAdmin["data/shaders/basic"];
    renderable.material = { {0.3f, 0.3f, 0.3f}, {0.5f, 0.5f, 0.5f}, {0.f, 0.f, 0.f}, 32.0f };

    const auto size = renderable.mesh->getSize();
    registry.emplace<RectCollider>(asteroid, RectCollider{ -size / 2.f, size });
}

void createAsteroidSpawner(entt::registry& registry, const AsteroidSpawnerData& spawnerData)
{
    const auto test = registry.create();
    registry.emplace<AsteroidSpawner>(test, AsteroidSpawner{ 
        Timer{ Fseconds(spawnerData.asteroidFrequency)},
        spawnerData.asteroidFrequency,
        spawnerData.asteroidFrequencyIncrease,
        spawnerData.omegaRange,
        spawnerData.prefab
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
    const auto bg = registry.create();
    registry.emplace<Transform>(bg);
    registry.emplace<Kinetic>(bg, Kinetic{ {0, 0, 0}, { - playerForwardVelocity / 360.f, 0.f, 0.f} });
    auto& skybox = registry.emplace<Skybox>(bg,
        std::make_unique<Cubemap>(facesFilePaths),
        g_ShaderAdmin["data/shaders/skybox"]
    );
    skybox.shader->bind();
    skybox.shader->setUniform("u_Skybox", 0);
}

void createAsteroidExplosion(entt::registry& registry, glm::vec3 position, Fseconds explosionDuration)
{
    const auto explosion = registry.create();
    registry.emplace<Transform>(explosion, position);
    registry.emplace<TimedObsolete>(explosion, explosionDuration + Fseconds(1));
    auto& emmitter = registry.emplace<ParticlesEmmiter>(explosion);
    emmitter.duration = explosionDuration;
    emmitter.emitRate = 2000;
    emmitter.generators.emplace_back(new PositionGenerator(glm::vec3{ -0.5, -0.5, -0.5 }, glm::vec3{ 0.5, 0.5, 0.5 }));
    emmitter.generators.emplace_back(new ColorGenerator(
        glm::vec4{ 1.f, 1.f, 0.7f, 1.0 },
        glm::vec4{ 1.f, 0.75f, 0.44f, 1.0 },
        glm::vec4{ 1.f, 0.35f, 0.078f, 0.0 },
        glm::vec4{ 1.f, 0.137f, 0.137f, 0.0 }
    ));
    emmitter.generators.emplace_back(new VelocityGenerator(glm::vec3{ -5.f, -5.f, -5.f}, glm::vec3{ 5.f, 5.f, 5.f }));
    emmitter.generators.emplace_back(new TimeGenerator(Fseconds(0.75f), Fseconds(1.f)));

    auto& container = registry.emplace<ParticlesContainer>(explosion);
    container.initialize(g_ShaderAdmin["data/shaders/particle"], 1000);
}

void createBulletExplosion(entt::registry& registry, glm::vec3 position)
{
    const auto explosion = registry.create();
    registry.emplace<Transform>(explosion, position);
    registry.emplace<TimedObsolete>(explosion, Fseconds(2.f));
    auto& emmitter = registry.emplace<ParticlesEmmiter>(explosion);
    emmitter.duration = Fseconds(0.1f);
    emmitter.emitRate = 1000;
    emmitter.generators.emplace_back(new PositionGenerator(glm::vec3{ -0.5, -0.5, -0.5 }, glm::vec3{ 0.5, 0.5, 0.5 }));
    emmitter.generators.emplace_back(new ColorGenerator(
        glm::vec4{ 0.09f, 0.31f, 0.67f, 1.0 },
        glm::vec4{ 0.f, 0.2f, 0.58f, 1.0 },
        glm::vec4{ 0.45f, 0.31, 0.93f, 0.0 },
        glm::vec4{ 0.52f, 0.8f, 0.98f, 0.0 }
    ));
    emmitter.generators.emplace_back(new VelocityGenerator(glm::vec3{ -1.f, -1.f, -1.f }, glm::vec3{ 1.f, 1.f, 1.f }));
    emmitter.generators.emplace_back(new TimeGenerator(Fseconds(0.75f), Fseconds(1.f)));

    auto& container = registry.emplace<ParticlesContainer>(explosion);
    container.initialize(g_ShaderAdmin["data/shaders/particle"], 250);
}