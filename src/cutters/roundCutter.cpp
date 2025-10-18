#include "cutters/roundCutter.hpp"

RoundCutter::RoundCutter() :
	Cutter{CutterType::round}
{
	resetPos();
	updateElements();
}

void RoundCutter::renderElements() const
{
	m_cyllinder.render();
	m_dome.render();
}

void RoundCutter::updateElements()
{
	float radius = getDiameter() / 2.0f;
	glm::vec3 offset{0, radius, 0};
	glm::vec3 pos = getPos();

	m_cyllinder.setScale({radius, 7 * radius, radius});
	m_cyllinder.setPos(pos + offset);
	m_dome.setScale({radius, radius, radius});
	m_dome.setPos(pos + offset);
}
