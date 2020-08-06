#pragma once

#include <string>
#include <glm/vec4.hpp>
#include "GLDebug.hpp"

class Shader
{
public:
	Shader(const std::string& vertexSource, const std::string& fragmentSource);

	Shader(Shader&)noexcept = delete;
	Shader& operator=(Shader&)noexcept = delete;

	~Shader();

	void bind()const;
	void unbind()const;

	template<class T>
	void setUniform(const std::string& name, T t)
	{
		static_assert(false);
	}

	template<>
	void setUniform<int>(const std::string& name, int v)
	{
		CHECK_GL(glUniform1i(getLocation(name), v));
	}

	template<>
	void setUniform<glm::vec4>(const std::string& name, glm::vec4 v)
	{
		CHECK_GL(glUniform4f(getLocation(name), v.r, v.g, v.b, v.a));
	}

	int getLocation(const std::string& name);

private:
	unsigned int m_Id;
};