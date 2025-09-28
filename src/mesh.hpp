#pragma once

class Mesh
{
public:
	Mesh(int vertexCount);
	~Mesh();
	void render() const;
	void setVertexCount(int vertexCount);

private:
	unsigned int m_VAO{};
	int m_vertexCount{};
};
