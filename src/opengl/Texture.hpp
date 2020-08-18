#pragma once

#include <string>
#include <memory>

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	[[nodiscard]] int getWidth()const { return m_Width; }
	[[nodiscard]] int getHeight()const { return m_Height; }

private:
	unsigned int m_Id = 0;
	int m_Width = 0;
	int m_Height = 0;
	int m_BPP = 0;
};

using SharedTexture = std::shared_ptr<Texture>;