#pragma once

#include <glm/glm.hpp>

#include <vector>

class Mesh
{
public:
	struct Vertex
	{
		glm::vec3 pos{};
		glm::vec3 normalVec{};
	};

	Mesh(const std::vector<Vertex>& vertices);
	~Mesh();
	void render() const;

private:
	int m_vertexCount{};
	unsigned int m_VBO{};
	unsigned int m_VAO{};

	void createVBO(const std::vector<Vertex>& vertices);
};
