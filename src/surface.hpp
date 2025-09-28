#pragma once

#include <glm/glm.hpp>

#include <vector>

class Surface
{
public:
	class Column
	{
	public:
		Column(Surface& surface, int x);
		float operator[](int y) const;
		float& operator[](int y);

	private:
		Surface& m_surface;
		int m_x{};
	};

	Surface(const glm::ivec2& size);

	void resize(const glm::ivec2& size);
	const Column& operator[](int x) const;
	Column& operator[](int x);
	const std::vector<float>& surface() const;
	std::vector<float> rectangle(int xOffset, int yOffset, int width, int height) const;

private:
	glm::ivec2 m_size{};
	std::vector<float> m_surface{};
	std::vector<Column> m_columns{};

	int index(int x, int y) const;
};
