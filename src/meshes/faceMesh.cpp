#include "meshes/faceMesh.hpp"

#include <glad/glad.h>

FaceMesh::FaceMesh()
{
	glGenVertexArrays(1, &m_VAO);
}

FaceMesh::~FaceMesh()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void FaceMesh::render(int vertexCount) const
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindVertexArray(0);
}
