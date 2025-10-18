#pragma once

#include <glm/glm.hpp>

#include <vector>

class PolylineMesh
{
public:
	PolylineMesh(const std::vector<glm::vec3>& vertices);
	~PolylineMesh();
	void render() const;

private:
	int m_vertexCount{};
	unsigned int m_VBO{};
	unsigned int m_VAO{};

	void createVBO(const std::vector<glm::vec3>& vertices);
};
