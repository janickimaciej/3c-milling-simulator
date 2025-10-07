#pragma once

#include "cutterElement.hpp"
#include "cutterType.hpp"

#include <glm/glm.hpp>

class Cutter
{
public:
	Cutter(CutterType type);
	virtual ~Cutter() = default;
	CutterType type() const;
	void render() const;
	float getDiameter() const;
	void setDiameter(float diameter);
	float getMillingHeight() const;
	void setMillingHeight(float millingHeight);
	float getMaxMillingDepth() const;
	void setMaxMillingDepth(float maxMillingDepth);
	float getSpeed() const;
	void setSpeed(float speed);
	glm::vec3 getPos() const;
	void setPos(const glm::vec3& pos);

	virtual void renderElements() const = 0;
	virtual void updateElementsScale() = 0;
	virtual void updateElementsPos() = 0;

private:
	CutterType m_type{};
	float m_diameter = 10;
	float m_millingHeight = 20;
	float m_maxMillingDepth = 30;
	float m_speed = 250;
	glm::vec3 m_pos{0, 100, 0};
};
