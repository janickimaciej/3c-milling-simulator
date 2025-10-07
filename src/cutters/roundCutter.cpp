#include "cutters/roundCutter.hpp"

RoundCutter::RoundCutter() :
	Cutter{CutterType::round}
{
	updateElementsScale();
	updateElementsPos();
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

	m_cyllinder.setScale({radius, maxMillingDepth, radius});
	m_dome.setScale({radius, radius, radius});
}

void RoundCutter::updateElementsPos()
{
	glm::vec3 pos = getPos();

	m_cyllinder.setPos(pos);
	m_dome.setPos(pos);
}
