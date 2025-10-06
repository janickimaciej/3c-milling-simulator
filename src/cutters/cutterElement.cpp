#include "cutters/cutterElement.hpp"

#include "objParser.hpp"
#include "shaderPrograms.hpp"

#include <glm/gtc/matrix_inverse.hpp>

CutterElement::CutterElement(const std::string& path)
{
	m_mesh = std::make_unique<Mesh>(ObjParser::parse(path));
}

void CutterElement::render() const
{
	ShaderPrograms::cutter->setUniform("modelMatrix", m_modelMatrix);
	ShaderPrograms::cutter->setUniform("inverseTransposeModelMatrix",
		glm::inverseTranspose(glm::mat3(m_modelMatrix)));
	m_mesh->render();
}

void CutterElement::setScale(const glm::vec3& scale)
{
	m_scale = scale;
	updateModelMatrix();
}

void CutterElement::setPos(const glm::vec3& pos)
{
	m_pos = pos;
	updateModelMatrix();
}

void CutterElement::updateModelMatrix()
{
	glm::mat4 scaleMatrix
	{
		m_scale.x, 0, 0, 0,
		0, m_scale.y, 0, 0,
		0, 0, m_scale.z, 0,
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
