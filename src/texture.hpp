#pragma once

#include "surface.hpp"

#include <glm/glm.hpp>

#include <vector>

class Texture
{
public:
	Texture(const glm::ivec2& size, const float* data);
	Texture(const Texture&) = delete;
	Texture(Texture&& texture) noexcept;
	~Texture();

	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&& texture) noexcept;

	void use() const;
	unsigned int getId() const;
	void resize(const glm::ivec2& size, const float* data);
	void update(int xOffset, int yOffset, int width, int height, const Surface& surface) const;

private:
	bool m_invalid = false;

	unsigned int m_id{};
	glm::ivec2 m_size{};
};
