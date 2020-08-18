#pragma once

#include <glm/vec3.hpp>

struct Light
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};