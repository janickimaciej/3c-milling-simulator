#pragma once

#include "scene.hpp"

#include <glm/glm.hpp>

#include <string>

class LeftPanel
{
public:
	static constexpr int width = 272;

	LeftPanel(Scene& scene, const glm::ivec2& viewportSize);
	void update();

private:
	Scene& m_scene;
	const glm::ivec2& m_viewportSize;

	static constexpr int m_maxFileNameLength = 64;
	std::array<char, m_maxFileNameLength> m_toolpathsFilePath{};

	void updateSimulationSpeed();
	void updateMaterialSize();
	void updateGridSize();
	void updateBaseY();
	void updateCutterType();
	void updateCutterDiameter();
	void updateCutterMillingHeight();
	void updateCutterSpeed();
	void updateToolpathsFilePath();
	void updateRenderCutter();
	void updateRenderToolpath();
	void updateButtons();
	void updateWarnings();

	void separator();
};
