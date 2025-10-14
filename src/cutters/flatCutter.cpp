#include "cutters/flatCutter.hpp"

FlatCutter::FlatCutter() :
	Cutter{CutterType::flat}
{
	updateElementsScale();
	resetPos();
}

void FlatCutter::renderElements() const
{
	m_cyllinder.render();
}

void FlatCutter::updateElementsScale()
{
	float radius = getDiameter() / 2.0f;
	float maxMillingDepth = getMaxMillingDepth();

	m_cyllinder.setScale({radius, maxMillingDepth, radius});
}

void FlatCutter::updateElementsPos()
{
	glm::vec3 pos = getPos();

	m_cyllinder.setPos(pos);
}
