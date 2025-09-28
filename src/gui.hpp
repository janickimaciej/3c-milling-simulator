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

	void update();
	void render();
};
