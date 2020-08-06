#include "Shader.hpp"

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

int Shader::getLocation(const std::string& name)
{
	return glGetUniformLocation(m_Id, name.c_str());
}
