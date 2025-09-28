#include "scene.hpp"

static constexpr float fovYDeg = 60.0f;
static constexpr float nearPlane = 0.1f;
static constexpr float farPlane = 1000.0f;

Scene::Scene(const glm::ivec2& windowSize) :
	m_camera{windowSize, fovYDeg, nearPlane, farPlane, m_shaderProgram},
	m_mesh{6 * m_gridSize.x * m_gridSize.y},
	m_surface{m_gridSize},
	m_heightMap{{m_gridSize.x + 1, m_gridSize.y + 1}, m_surface.surface().data()}
{ }

void Scene::update()
{
	static int ind = 0;

	m_surface[ind % (m_gridSize.x + 1)][ind / (m_gridSize.x + 1)] = -0.1f;
	m_heightMap.update(0, 0, m_gridSize.x + 1, m_gridSize.y + 1, m_surface.rectangle(0, 0, m_gridSize.x + 1, m_gridSize.y + 1));

	++ind;
}

void Scene::render()
{
	static constexpr glm::vec3 backgroundColor{0.1f, 0.1f, 0.1f};
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);

	m_camera.use();

	m_shaderProgram.use();
	m_shaderProgram.setUniform("materialSize", glm::vec2(m_materialSize));
	m_shaderProgram.setUniform("gridSize", m_gridSize);
	m_heightMap.use();
	m_mesh.render();
}

void Scene::updateWindowSize()
{
	m_camera.updateWindowSize();
}

void Scene::moveXCamera(float x)
{
	m_camera.moveX(x);
}

void Scene::moveYCamera(float y)
{
	m_camera.moveY(y);
}

void Scene::addPitchCamera(float pitchRad)
{
	m_camera.addPitch(pitchRad);
}

void Scene::addYawCamera(float yawRad)
{
	m_camera.addYaw(yawRad);
}

void Scene::zoomCamera(float zoom)
{
	m_camera.zoom(zoom);
}
