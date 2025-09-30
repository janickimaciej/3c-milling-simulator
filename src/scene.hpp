#pragma once

#include "camera.hpp"
#include "cutter.hpp"
#include "faceMesh.hpp"
#include "shaderProgram.hpp"
#include "surface.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>

class Scene
{
public:
	Scene(const glm::ivec2& windowSize);

	void update();
	void render();
	void updateWindowSize();
	
	void moveXCamera(float x);
	void moveYCamera(float y);
	void addPitchCamera(float pitchRad);
	void addYawCamera(float yawRad);
	void zoomCamera(float zoom);

private:
	Camera m_camera;
	
	glm::vec3 m_materialSize{400, 100, 200};
	glm::ivec2 m_gridSize{20, 10};

	Surface m_surface;
	Texture m_heightMap;
	FaceMesh m_faceMesh{};

	Cutter m_flatCutter{"res/flatCutter.obj"};
	Cutter m_roundCutter{"res/roundCutter.obj"};

	Cutter* m_activeCutter = &m_roundCutter;
};
