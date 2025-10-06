#pragma once

#include "cutters/cutterElement.hpp"
#include "cutters/cutter.hpp"
#include "objParser.hpp"

class RoundCutter : public Cutter
{
public:
	RoundCutter();

	virtual void renderElements() const override;
	virtual void updateElementsScale() override;
	virtual void updateElementsPos() override;

private:
	CutterElement m_cyllinder{"res/meshes/roundCutterCyllinder.obj"};
	CutterElement m_dome{"res/meshes/roundCutterDome.obj"};
};
