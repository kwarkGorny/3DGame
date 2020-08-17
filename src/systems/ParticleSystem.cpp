#include "ParticleSystem.hpp"

#include "components/Transform.hpp"
#include "components/Particles.hpp"

#include "contexts/Camera.hpp"

#include <entt/entity/registry.hpp>

#include "opengl/GLDebug.hpp"

namespace
{
	void onParticleContainerDestroyed(entt::registry& r, entt::entity e)
	{
		r.get<ParticlesContainer>(e).deinitialize();
	}
}

namespace ParticleSystem
{
	void setup(entt::registry& registry)
	{
		registry.on_destroy<ParticlesContainer>().connect<&onParticleContainerDestroyed>();
	}

	void update(entt::registry& registry, Fseconds dt)
	{
		const auto dtCount = dt.count();
		const auto emmiters = registry.view<ParticlesEmmiter, ParticlesContainer>();
		for (const auto entity : emmiters)
		{
			auto& emmiter = emmiters.get<ParticlesEmmiter>(entity);
			if (!emmiter.infinite)
			{
				emmiter.duration -= dt;
				if (emmiter.duration <= Fseconds::zero())
				{
					registry.remove<ParticlesEmmiter>(entity);
					continue;
				}
			}

			const std::size_t maxNewParticles = static_cast<size_t>(dtCount * emmiter.emitRate);
			if (maxNewParticles == 0)
			{
				continue;
			}
			auto& container = emmiters.get<ParticlesContainer>(entity);
			const std::size_t startId = container.countAlive;
			const std::size_t endId = std::min(startId + maxNewParticles, container.count - 1);
			for (auto& gen : emmiter.generators)
			{
				gen->generate(dt, container, startId, endId);
			}
			for (std::size_t i = startId; i < endId; ++i)
			{
				container.wake(i);
			}
		}

		const auto containers = registry.view<ParticlesContainer>();
		for (const auto entity : containers)
		{
			auto& particles = containers.get(entity);
			if (particles.countAlive == 0)
			{
				continue;
			}
			auto& positions = particles.positions;
			auto& velocities = particles.velocities;
			auto& colors = particles.colors;
			const auto& startColors = particles.startColors;
			const auto& endColors = particles.endColors;
			auto& timers = particles.timers;

			for (int id = 0; id < particles.countAlive; ++id)
			{
				timers[id].dt += dt;
				timers[id].progress = timers[id].dt.count() / timers[id].duration.count();
				if (timers[id].dt > timers[id].duration)
				{
					particles.kill(id);
				}
			}
			for (int id = 0; id < particles.countAlive; ++id)
			{
				positions[id] += velocities[id] * dtCount;
			}
			for (int id = 0; id < particles.countAlive; ++id)
			{
				colors[id] = glm::mix(startColors[id], endColors[id], timers[id].progress);
			}
		}
	}

	void draw(entt::registry& registry)
	{
		const auto cameraPtr = registry.try_ctx<Camera>();
		if (!cameraPtr)
		{
			return;
		}
		const auto& camera = *cameraPtr;

		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_PROGRAM_POINT_SIZE);

		const auto view = registry.view<ParticlesContainer, Transform>();
		for (const auto entity : view)
		{
			const auto& particles = view.get<ParticlesContainer>(entity);
			if (particles.initialized && particles.countAlive == 0)
			{
				continue;
			}
			const auto& t = view.get<Transform>(entity);

			glBindBuffer(GL_ARRAY_BUFFER, particles.positionBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, particles.countAlive * sizeof(ParticlesContainer::PositionType), particles.positions.data());

			glBindBuffer(GL_ARRAY_BUFFER, particles.colorBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, particles.countAlive * sizeof(ParticlesContainer::ColorType), particles.colors.data());

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(particles.vao);

			particles.shader->bind();
			particles.shader->setUniform("u_Model", t.toModelMatrix());
			particles.shader->setUniform("u_View", camera.view);
			particles.shader->setUniform("u_Projection", camera.projection);

			glDrawArrays(GL_POINTS, 0, particles.countAlive);
		}

		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_PROGRAM_POINT_SIZE);
	}
}