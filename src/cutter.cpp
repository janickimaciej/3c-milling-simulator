#include "cutter.hpp"

#include "objParser.hpp"
#include "shaderPrograms.hpp"

Cutter::Cutter(const std::string& meshPath)
{
	std::vector<Vertex> vertices = ObjParser::parse(meshPath);
	m_mesh = std::make_unique<Mesh>(vertices);
	updateModelMatrix();
}

void Cutter::render() const
{
	ShaderPrograms::cutter->use();
	ShaderPrograms::cutter->setUniform("modelMatrix", m_modelMatrix);
	ShaderPrograms::cutter->setUniform("millingHeight", m_pos.y + m_millingHeight);
	m_mesh->render();
}

void Cutter::setRadius(float radius)
{
	m_radius = radius;
	updateModelMatrix();
}

void Cutter::setMillingHeight(float millingHeight)
{
	m_millingHeight = millingHeight;
}

void Cutter::setPos(const glm::vec3& pos)
{
	m_pos = pos;
	updateModelMatrix();
}

void Cutter::updateModelMatrix()
{
	glm::mat4 scaleMatrix
	{
		m_radius, 0, 0, 0,
		0, m_radius, 0, 0,
		0, 0, m_radius, 0,
		0, 0, 0, 1
	};

	glm::mat4 posMatrix
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		m_pos.x, m_pos.y, m_pos.z, 1
	};

	m_modelMatrix = posMatrix * scaleMatrix;
}
