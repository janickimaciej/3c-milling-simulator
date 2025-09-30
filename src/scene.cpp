#include "scene.hpp"

#include "shaderPrograms.hpp"
#include "side.hpp"

#include <cstdlib>

static constexpr float fovYDeg = 60.0f;
static constexpr float nearPlane = 0.1f;
static constexpr float farPlane = 1000.0f;

Scene::Scene(const glm::ivec2& windowSize) :
	m_camera{windowSize, fovYDeg, nearPlane, farPlane},
	m_surface{m_gridSize},
	m_heightMap{{m_gridSize.x + 1, m_gridSize.y + 1}, m_surface.surface().data()}
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void Scene::update()
{
	static int ind = 0;

	if (ind < (m_gridSize.x + 1) * (m_gridSize.y + 1))
	{
		float depth = static_cast<float>(std::rand()) / RAND_MAX / 10;
		m_surface[ind % (m_gridSize.x + 1)][ind / (m_gridSize.x + 1)] = -depth;
		m_heightMap.update(0, 0, m_gridSize.x + 1, m_gridSize.y + 1,
			m_surface.rectangle(0, 0, m_gridSize.x + 1, m_gridSize.y + 1));

		++ind;
	}
}

void Scene::render()
{
	static constexpr glm::vec3 backgroundColor{0.1f, 0.1f, 0.1f};
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_camera.use();

	ShaderPrograms::topFace->use();
	ShaderPrograms::topFace->setUniform("materialSize", m_materialSize);
	ShaderPrograms::topFace->setUniform("gridSize", m_gridSize);
	m_heightMap.use();
	m_faceMesh.render(6 * m_gridSize.x * m_gridSize.y);

	ShaderPrograms::sideFace->use();
	ShaderPrograms::sideFace->setUniform("materialSize", m_materialSize);
	ShaderPrograms::sideFace->setUniform("gridSize", m_gridSize);
	m_heightMap.use();
	ShaderPrograms::sideFace->setUniform("side", static_cast<int>(Side::left));
	m_faceMesh.render(6 * m_gridSize.y);
	ShaderPrograms::sideFace->setUniform("side", static_cast<int>(Side::right));
	m_faceMesh.render(6 * m_gridSize.y);
	ShaderPrograms::sideFace->setUniform("side", static_cast<int>(Side::back));
	m_faceMesh.render(6 * m_gridSize.x);
	ShaderPrograms::sideFace->setUniform("side", static_cast<int>(Side::front));
	m_faceMesh.render(6 * m_gridSize.x);

	ShaderPrograms::bottomFace->use();
	ShaderPrograms::bottomFace->setUniform("materialSize", m_materialSize);
	m_faceMesh.render(6);
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
