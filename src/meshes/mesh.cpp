#include "meshes/mesh.hpp"

#include <glad/glad.h>

Mesh::Mesh(const std::vector<Vertex>& vertices)
{
	glGenVertexArrays(1, &m_VAO);
	createVBO(vertices);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void Mesh::render() const
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
	glBindVertexArray(0);
}

void Mesh::createVBO(const std::vector<Vertex>& vertices)
{
	m_vertexCount = static_cast<int>(vertices.size());

	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, normalVec)));
	glEnableVertexAttribArray(1);

	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
		vertices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}
