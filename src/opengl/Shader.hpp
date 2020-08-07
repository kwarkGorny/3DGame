#pragma once

#include <string>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
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


	void setUniform(const std::string& name, int v)
	{
		CHECK_GL(glUniform1i(getLocation(name), v));
	}

	void setUniform(const std::string& name, const glm::vec4& v)
	{
		CHECK_GL(glUniform4f(getLocation(name), v.r, v.g, v.b, v.a));
	}

	void setUniform(const std::string& name, const glm::mat4& m)
	{
		CHECK_GL(glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, &m[0][0]));
	}

	int getLocation(const std::string& name);

private:
	unsigned int m_Id;
};