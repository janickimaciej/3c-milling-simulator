#pragma once

#include "cutterElement.hpp"

#include <glm/glm.hpp>

class Cutter
{
public:
	virtual ~Cutter() = default;
	void render() const;
	float getDiameter() const;
	void setDiameter(float diameter);
	float getMillingHeight() const;
	void setMillingHeight(float millingHeight);
	float getMaxMillingDepth() const;
	void setMaxMillingDepth(float maxMillingDepth);
	glm::vec3 getPos() const;
	void setPos(const glm::vec3& pos);

	virtual void renderElements() const = 0;
	virtual void updateElementsScale() = 0;
	virtual void updateElementsPos() = 0;

private:
	float m_diameter = 10;
	float m_millingHeight = 20;
	float m_maxMillingDepth = 30;
	glm::vec3 m_pos{0, 100, 0};
};
