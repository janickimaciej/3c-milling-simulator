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
	ShaderPrograms::cutter->setUniform("millingHeight", getPos().y + getMillingHeight());

	renderElements();
}

float Cutter::getDiameter() const
{
	return m_diameter;
}

void Cutter::setDiameter(float diameter)
{
	m_diameter = diameter;
	updateElements();
}

float Cutter::getMillingHeight() const
{
	return m_millingHeightToDiameterRatio * getDiameter();
}

void Cutter::setMillingHeight(float millingHeight)
{
	m_millingHeightToDiameterRatio = millingHeight / getDiameter();
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
	updateElements();
}

void Cutter::resetPos()
{
	setPos({0, 66, 0});
}
