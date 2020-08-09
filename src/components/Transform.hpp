#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform
{
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 rotation = glm::vec3(0.f);
	glm::vec3 scale = glm::vec3(1.f);

	inline glm::mat4 toModelView()const 
	{
		glm::mat4 model(1.f);
		model = glm::translate(model, position);
		model = glm::rotate(model, rotation.x, glm::vec3(1.f, 0.f, 0.f));
		model = glm::rotate(model, rotation.y, glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, rotation.z, glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, scale);
		return model;
	}
};