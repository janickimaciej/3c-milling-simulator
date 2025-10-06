#include "texture.hpp"

#include <glad/glad.h>

Texture::Texture(const glm::ivec2& size, const float* data) :
	m_size{size}
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	resize(size, data);
}

Texture::Texture(Texture&& texture) noexcept :
	m_id{texture.m_id},
	m_size{texture.m_size}
{
	texture.m_invalid = true;
}

Texture::~Texture()
{
	if (!m_invalid)
	{
		glDeleteTextures(1, &m_id);
	}
}

Texture& Texture::operator=(Texture&& texture) noexcept
{
	m_id = texture.m_id;
	m_size = texture.m_size;
	texture.m_invalid = true;
	return *this;
}

void Texture::use() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

unsigned int Texture::getId() const
{
	return m_id;
}

void Texture::resize(const glm::ivec2& size, const float* data)
{
	m_size = size;

	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_size.x, m_size.y, 0, GL_RED, GL_FLOAT, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::update(int xOffset, int yOffset, int width, int height,
	const std::vector<float>& data) const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, GL_RED, GL_FLOAT,
		data.data());
	glBindTexture(GL_TEXTURE_2D, 0);
}
