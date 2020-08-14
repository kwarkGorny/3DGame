#pragma once

#include <string>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <memory>
#include <unordered_map>

#include "Material.hpp"
#include "Light.hpp"

class Shader
{
public:
	Shader(const std::string& vertexSource, const std::string& fragmentSource);

	Shader(Shader&)noexcept = delete;
	Shader& operator=(Shader&)noexcept = delete;

	~Shader();

	void bind()const;
	void unbind()const;


	void setUniform(const std::string& name, int v)const;
	void setUniform(const std::string& name, float v)const;
	void setUniform(const std::string& name, const glm::vec3& v)const;
	void setUniform(const std::string& name, const glm::vec4& v)const;
	void setUniform(const std::string& name, const glm::mat4& m)const;
	void setUniform(const std::string& name, const Material& m)const;
	void setUniform(const std::string& name, const Light& l)const;

	int getLocation(const std::string& name)const;

private:
	unsigned int m_Id;
	mutable std::unordered_map<std::string, int> m_LocationsMap;
};

using SharedShader = std::shared_ptr<Shader>;