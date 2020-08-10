#include "Shader.hpp"


#include "basic/Logger.hpp"
#include "GLDebug.hpp"

#include <iostream>
#include <memory>

namespace
{
	GLuint compileShader(unsigned int type, const std::string& source)
	{
		CHECK_GL(const auto id = glCreateShader(type));
		const auto str = source.c_str();
		CHECK_GL(glShaderSource(id, 1, &str, nullptr));
		CHECK_GL(glCompileShader(id));

		int result;
		CHECK_GL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int length;
			CHECK_GL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			std::unique_ptr<char[]> str(new char[length]);
			CHECK_GL(glGetShaderInfoLog(id, length, &length, str.get()));
			std::cout << "FAILED TO COMPILED SHADER: " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " " << str.get() << '\n';
		}

		return id;
	}

	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		CHECK_GL(const auto program = glCreateProgram());
		const auto vs = compileShader(GL_VERTEX_SHADER, vertexShader);
		const auto fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

		CHECK_GL(glAttachShader(program, vs));
		CHECK_GL(glAttachShader(program, fs));

		CHECK_GL(glLinkProgram(program));

		CHECK_GL(glDetachShader(program, vs));
		CHECK_GL(glDetachShader(program, fs));

		CHECK_GL(glValidateProgram(program));

		CHECK_GL(glDeleteShader(vs));
		CHECK_GL(glDeleteShader(fs));
		return program;
	}
}

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
	: m_Id(CreateShader(vertexSource, fragmentSource))
{
}

Shader::~Shader()
{
	CHECK_GL(glDeleteProgram(m_Id));
}

void Shader::bind() const
{
	CHECK_GL(glUseProgram(m_Id));
}

void Shader::unbind() const
{
	CHECK_GL(glUseProgram(0));
}

void Shader::setUniform(const std::string& name, int v)const
{
	CHECK_GL(glUniform1i(getLocation(name), v));
}

void Shader::setUniform(const std::string& name, float v)const
{
	CHECK_GL(glUniform1f(getLocation(name), v));
}

void Shader::setUniform(const std::string& name, const glm::vec3& v)const
{
	CHECK_GL(glUniform3f(getLocation(name), v.r, v.g, v.b));
}

void Shader::setUniform(const std::string& name, const glm::vec4& v)const
{
	CHECK_GL(glUniform4f(getLocation(name), v.r, v.g, v.b, v.a));
}

void Shader::setUniform(const std::string& name, const glm::mat4& m)const
{
	CHECK_GL(glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, &m[0][0]));
}

void Shader::setUniform(const std::string& name, const Material& m)const
{
	setUniform(name + ".ambient", m.ambient);
	setUniform(name + ".diffuse", m.diffuse);
	setUniform(name + ".specular", m.specular);
	setUniform(name + ".shininess", m.shininess);
}

void Shader::setUniform(const std::string& name, const Light& l)const
{
	setUniform(name + ".position", l.position);
	setUniform(name + ".ambient", l.ambient);
	setUniform(name + ".diffuse", l.diffuse);
	setUniform(name + ".specular", l.specular);
}

int Shader::getLocation(const std::string& name)const
{
	const auto itr = m_LocationsMap.find(name);
	if (itr != m_LocationsMap.end())
	{
		return itr->second;
	}
	const int loc = glGetUniformLocation(m_Id, name.c_str());
	logger::error(loc == -1, "such shader location dont exist {}", name);
	m_LocationsMap.emplace(name, loc);
	return loc;
}
