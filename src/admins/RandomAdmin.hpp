#pragma once

#include <random>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/scalar_constants.hpp>

class RandomAdmin
{
public:
	glm::vec3 getDirection()
	{
		std::uniform_real_distribution<float> zRandomizer(-1.f, 1.f);
		std::uniform_real_distribution<float> thetaRandomizer(0, 2 * glm::pi<float>());
		const float theta = thetaRandomizer(m_Generator);
		const float z = zRandomizer(m_Generator);
		const float v = std::sqrt(1 - (z * z));
		return glm::vec3(v * std::cos(theta), v * std::sin(theta), z);
	}

	int getUniform(int start, int end) 
	{
		 std::uniform_int_distribution<int> distrib(start, end);
		 return distrib(m_Generator);
	}

	float getUniform(glm::vec2 range)
	{
		std::uniform_real_distribution<float> distrib(range.x, range.y);
		return distrib(m_Generator);
	}
private:
	std::mt19937_64 m_Generator = std::mt19937_64{ std::random_device{}() };
};

inline RandomAdmin g_RandomAdmin;