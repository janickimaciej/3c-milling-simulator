#include "cutters/roundCutter.hpp"

RoundCutter::RoundCutter() :
	Cutter{CutterType::round}
{
	resetPos();
	updateElementsScale();
}

void RoundCutter::renderElements() const
{
	m_cyllinder.render();
	m_dome.render();
}

void RoundCutter::updateElementsScale()
{
	float radius = getDiameter() / 2.0f;
	float maxMillingDepth = getMaxMillingDepth();

	m_cyllinder.setScale({radius, maxMillingDepth - radius, radius});
	m_dome.setScale({radius, radius, radius});
}

void RoundCutter::updateElementsPos()
{
	glm::vec3 pos = getPos();

	glm::vec3 offset{0, getDiameter() / 2.0f, 0};
	m_cyllinder.setPos(pos + offset);
	m_dome.setPos(pos + offset);
}
