#include "meshes/polylineMesh.hpp"

#include <glad/glad.h>

PolylineMesh::PolylineMesh(const std::vector<glm::vec3>& vertices)
{
	glGenVertexArrays(1, &m_VAO);
	createVBO(vertices);
}

PolylineMesh::~PolylineMesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void PolylineMesh::render() const
{
	glBindVertexArray(m_VAO);
	glPointSize(3);
	glDrawArrays(GL_POINTS, 0, m_vertexCount);
	glDrawArrays(GL_LINE_STRIP, 0, m_vertexCount);
	glBindVertexArray(0);
}

void PolylineMesh::createVBO(const std::vector<glm::vec3>& vertices)
{
	m_vertexCount = static_cast<int>(vertices.size());

	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
		reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3)),
		vertices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}
