#include "cutters/cutter.hpp"

#include "objParser.hpp"
#include "shaderPrograms.hpp"

void Cutter::render() const
{
	ShaderPrograms::cutter->use();
	ShaderPrograms::cutter->setUniform("millingHeight", m_pos.y + m_millingHeight);

	renderElements();
}

float Cutter::getDiameter() const
{
	return m_diameter;
}

void Cutter::setDiameter(float diameter)
{
	m_diameter = diameter;
	updateElementsScale();
}

float Cutter::getMillingHeight() const
{
	return m_millingHeight;
}

void Cutter::setMillingHeight(float millingHeight)
{
	m_millingHeight = millingHeight;
}

float Cutter::getMaxMillingDepth() const
{
	return m_maxMillingDepth;
}

void Cutter::setMaxMillingDepth(float maxMillingDepth)
{
	m_maxMillingDepth = maxMillingDepth;
	updateElementsScale();
}

glm::vec3 Cutter::getPos() const
{
	return m_pos;
}

void Cutter::setPos(const glm::vec3& pos)
{
	m_pos = pos;
	updateElementsPos();
}
