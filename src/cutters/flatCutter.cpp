#include "cutters/flatCutter.hpp"

FlatCutter::FlatCutter() :
	Cutter{CutterType::flat}
{
	updateElements();
	resetPos();
}

void FlatCutter::renderElements() const
{
	m_cyllinder.render();
}

void FlatCutter::updateElements()
{
	float radius = getDiameter() / 2.0f;
	glm::vec3 pos = getPos();

	m_cyllinder.setScale({radius, 8 * radius, radius});
	m_cyllinder.setPos(pos);
}
