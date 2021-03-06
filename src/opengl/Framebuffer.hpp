#pragma once

#include <string>
#include <memory>

class [[nodiscard]] FrameBuffer
{
public:
	FrameBuffer(int width, int height);
	~FrameBuffer();

	void bind() const;
	void unbind() const;

	void bindTexture()const;
	void unbindTexture() const;

	[[nodiscard]] int getWidth()const { return m_Width; }
	[[nodiscard]] int getHeight()const { return m_Height; }

public:
	unsigned int m_Id = 0;
	unsigned int m_TextureId = 0;
	unsigned int m_ObjectId = 0;
	int m_Width = 0;
	int m_Height = 0;
};

using UniqueFrameBuffer = std::unique_ptr<FrameBuffer>;
using SharedFrameBuffer = std::shared_ptr<FrameBuffer>;