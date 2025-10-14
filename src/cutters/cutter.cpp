#include "cutters/cutter.hpp"

#include "objParser.hpp"
#include "shaderPrograms.hpp"

Cutter::Cutter(CutterType type) :
	m_type{type}
{ }

CutterType Cutter::type() const
{
	return m_type;
}

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

float Cutter::getSpeed() const
{
	return m_speed;
}

void Cutter::setSpeed(float speed)
{
	m_speed = speed;
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

void Cutter::resetPos()
{
	setPos({0, 66, 0});
}
