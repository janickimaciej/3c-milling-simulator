#include "scene.hpp"

#include "objParser.hpp"
#include "shaderPrograms.hpp"
#include "side.hpp"
#include "toolpathsFileParser.hpp"
#include "vertex.hpp"

#include <cstdlib>
#include <iterator>

static constexpr float fovYDeg = 60.0f;
static constexpr float nearPlane = 10.0f;
static constexpr float farPlane = 10000.0f;

Scene::Scene(const glm::ivec2& windowSize) :
	m_camera{windowSize, fovYDeg, nearPlane, farPlane},
	m_surface{m_gridSize, m_materialSize.y},
	m_heightMap{{m_gridSize.x + 1, m_gridSize.y + 1}, m_surface.surface().data()}
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	setCutterType(CutterType::flat);
}

void Scene::update()
{
	if (m_simulation)
	{
		m_simulation->step(m_simulationSpeed, m_materialSize, m_gridSize, m_surface,
			*m_activeCutter, m_heightMap);
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

	m_activeCutter->render();
}

void Scene::updateWindowSize()
{
	m_camera.updateWindowSize();
}

void Scene::loadToolpathsFile(const std::string& path)
{
	m_toolpath = std::make_unique<Toolpath>(ToolpathsFileParser::parse("res/toolpaths/" + path));
	m_simulation = std::make_unique<Simulation>(*m_toolpath);
}

void Scene::mill()
{
	if (m_simulation)
	{
		m_simulation->start();
	}
}

void Scene::stop()
{
	if (m_simulation)
	{
		m_simulation->stop();
	}
}

void Scene::millInstantly()
{
	if (m_simulation)
	{
		m_simulation->millInstantly(m_materialSize, m_gridSize, m_surface, *m_activeCutter,
			m_heightMap);
	}
}

void Scene::reset()
{
	m_surface.reset(m_gridSize, m_materialSize.y);
	m_heightMap.reset({m_gridSize.x + 1, m_gridSize.y + 1}, m_surface.surface().data());
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

float Scene::getSimulationSpeed() const
{
	return m_simulationSpeed;
}

void Scene::setSimulationSpeed(float simulationSpeed)
{
	m_simulationSpeed = simulationSpeed;
}

glm::vec3 Scene::getMaterialSize() const
{
	return m_materialSize;
}

void Scene::setMaterialSize(const glm::vec3& materialSize)
{
	m_materialSize = materialSize;
}

glm::ivec2 Scene::getGridSize() const
{
	return m_gridSize;
}

void Scene::setGridSize(const glm::ivec2& gridSize)
{
	m_gridSize = gridSize;
	reset();
}

CutterType Scene::getCutterType() const
{
	return m_activeCutter->type();
}

void Scene::setCutterType(CutterType cutterType)
{
	if (cutterType == CutterType::flat)
	{
		m_activeCutter = &m_flatCutter;
	}
	else
	{
		m_activeCutter = &m_roundCutter;
	}
}

float Scene::getCutterDiameter() const
{
	return m_activeCutter->getDiameter();
}

void Scene::setCutterDiameter(float diameter)
{
	m_activeCutter->setDiameter(diameter);
}

float Scene::getCutterMillingHeight() const
{
	return m_activeCutter->getMillingHeight();
}

void Scene::setCutterMillingHeight(float millingHeight)
{
	m_activeCutter->setMillingHeight(millingHeight);
}

float Scene::getMaxMillingDepth() const
{
	return m_activeCutter->getMaxMillingDepth();
}

void Scene::setMaxMillingDepth(float maxMillingDepth)
{
	m_activeCutter->setMaxMillingDepth(maxMillingDepth);
}

float Scene::getCutterSpeed() const
{
	return m_activeCutter->getSpeed();
}

void Scene::setCutterSpeed(float speed)
{
	m_activeCutter->setSpeed(speed);
}
