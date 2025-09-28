#include "mesh.hpp"

#include <glad/glad.h>

Mesh::Mesh(int vertexCount)
{
	setVertexCount(vertexCount);
	glGenVertexArrays(1, &m_VAO);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::render() const
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
	glBindVertexArray(0);
}

void Mesh::setVertexCount(int vertexCount)
{
	m_vertexCount = vertexCount;
}
