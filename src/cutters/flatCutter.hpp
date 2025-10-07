#pragma once

#include "cutters/cutterElement.hpp"
#include "cutters/cutter.hpp"
#include "cutters/cutterType.hpp"
#include "objParser.hpp"

class FlatCutter : public Cutter
{
public:
	FlatCutter();

	virtual void renderElements() const override;
	virtual void updateElementsScale() override;
	virtual void updateElementsPos() override;

private:
	CutterElement m_cyllinder{"res/meshes/flatCutterCyllinder.obj"};
};
