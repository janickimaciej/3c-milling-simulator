#pragma once

class FaceMesh
{
public:
	FaceMesh();
	~FaceMesh();
	void render(int vertexCount) const;

private:
	unsigned int m_VAO{};
};
