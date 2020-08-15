#pragma once

#include <string>
#include <memory>
#include <array>

class Cubemap
{
public:
	Cubemap(const std::array<std::string, 6>& facesFilePaths);
	~Cubemap();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	int getVAO()const { return m_VAO; }
	int getVBO()const { return m_VBO; }

private:
	unsigned int m_Id = 0;
	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
};

using UniqueCubemap = std::unique_ptr<Cubemap>;
