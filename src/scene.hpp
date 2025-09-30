#pragma once

#include "camera.hpp"
#include "mesh.hpp"
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
	
	glm::vec3 m_materialSize{2.0f, 0.5f, 1.0f};
	glm::ivec2 m_gridSize{20, 10};

	Surface m_surface;
	Texture m_heightMap;
	FaceMesh m_faceMesh{};
};
