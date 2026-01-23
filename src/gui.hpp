#pragma once

#include "scene.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

class GUI
{
public:
	static constexpr int width = 272;

	GUI(GLFWwindow* window, Scene& scene, const glm::ivec2& windowSize);
	~GUI();

	void update();
	void render();

private:
	Scene& m_scene;
	const glm::ivec2& m_windowSize;

	static constexpr int maxFileNameLength = 32;
	std::array<char, maxFileNameLength> m_toolpathsFilePath{};

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
