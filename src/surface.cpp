#include "surface.hpp"

#include <cstddef>

Surface::Column::Column(Surface& surface, int x) :
	m_x{x},
	m_surface{surface}
{ }

float Surface::Column::operator[](int y) const
{
	return m_surface.m_surface[m_surface.index(m_x, y)];
}

float& Surface::Column::operator[](int y)
{
	return m_surface.m_surface[m_surface.index(m_x, y)];
}

Surface::Surface(const glm::ivec2& size, float height)
{
	resize(size, height);
}

void Surface::resize(const glm::ivec2& size, float height)
{
	m_size = size;
	m_surface = std::vector<float>((size.x + 1) * (size.y + 1), height);

	m_columns.clear();
	for (int x = 0; x <= m_size.x; ++x)
	{
		m_columns.push_back({*this, x});
	}
}

const Surface::Column& Surface::operator[](int x) const
{
	return m_columns[x];
}

Surface::Column& Surface::operator[](int x)
{
	return m_columns[x];
}

const std::vector<float>& Surface::surface() const
{
	return m_surface;
}

std::vector<float> Surface::rectangle(int xOffset, int yOffset, int width, int height) const
{
	std::vector<float> rectangle{};
	for (int y = yOffset; y < yOffset + height; ++y)
	{
		rectangle.insert(rectangle.end(), m_surface.begin() + index(xOffset, y),
			m_surface.begin() + static_cast<std::size_t>(index(xOffset + width, y)));
	}
	return rectangle;
}

int Surface::index(int x, int y) const
{
	return (m_size.x + 1) * y + x;
}
