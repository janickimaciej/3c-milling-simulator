#include "scene.hpp"

#include "objParser.hpp"
#include "shaderPrograms.hpp"
#include "side.hpp"
#include "toolpathsFileParser.hpp"

#include <cstddef>
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

	setCutterType(CutterType::round);
	m_camera.addPitch(glm::radians(-60.0f));
	m_camera.zoom(2.0f);
}

void Scene::update()
{
	if (m_simulation != nullptr)
	{
		m_simulation->step(m_simulationSpeed, m_materialSize, m_gridSize, m_surface,
			*m_activeCutter, m_heightMap, m_baseY, m_warnings);
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
	ShaderPrograms::topFace->setUniform("baseY", m_baseY);
	m_heightMap.use();
	m_faceMesh.render(6 * m_gridSize.x * m_gridSize.y);

	ShaderPrograms::sideFace->use();
	ShaderPrograms::sideFace->setUniform("materialSize", m_materialSize);
	ShaderPrograms::sideFace->setUniform("gridSize", m_gridSize);
	ShaderPrograms::sideFace->setUniform("baseY", m_baseY);
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
	ShaderPrograms::bottomFace->setUniform("baseY", m_baseY);
	m_faceMesh.render(6);

	m_activeCutter->render();

	if (m_renderToolpath && m_toolpathMesh != nullptr)
	{
		ShaderPrograms::polyline->use();
		m_toolpathMesh->render();
	}
}

void Scene::updateWindowSize()
{
	m_camera.updateWindowSize();
}

void Scene::loadToolpathsFile(const std::string& path)
{
	std::vector<glm::vec3> toolpathPoss =
		ToolpathsFileParser::parse("res/toolpaths/" + path, m_warnings);
	if (toolpathPoss.size() > 0)
	{
		m_toolpath = std::make_unique<Toolpath>(toolpathPoss);
		m_toolpathMesh = std::make_unique<PolylineMesh>(m_toolpath->getPoss());
		m_simulation = std::make_unique<Simulation>(*m_toolpath);

		std::size_t lastDotPos = path.find_last_of('.');
		if (lastDotPos == path.size() - 4)
		{
			if (path[lastDotPos + 1] == 'k')
			{
				setCutterType(CutterType::round);
			}
			else if (path[lastDotPos + 1] == 'f')
			{
				setCutterType(CutterType::flat);
			}

			setCutterDiameter(static_cast<float>(std::stoi(path.substr(lastDotPos + 2))));
		}
	}
}

void Scene::mill()
{
	if (m_simulation != nullptr)
	{
		m_simulation->start();
	}
}

void Scene::stop()
{
	if (m_simulation != nullptr)
	{
		m_simulation->stop();
	}
}

void Scene::millInstantly()
{
	if (m_simulation != nullptr)
	{
		m_simulation->millInstantly(m_materialSize, m_gridSize, m_surface, *m_activeCutter,
			m_heightMap, m_baseY, m_warnings);
	}
}

void Scene::reset()
{
	m_surface.reset(m_gridSize, m_materialSize.y);
	m_heightMap.reset({m_gridSize.x + 1, m_gridSize.y + 1}, m_surface.surface().data());
	m_flatCutter.resetPos();
	m_roundCutter.resetPos();
	if (m_simulation != nullptr)
	{
		m_simulation->reset();
	}
	m_warnings = "";
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
	reset();
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

float Scene::getBaseY() const
{
	return m_baseY;
}

void Scene::setBaseY(float baseY)
{
	m_baseY = baseY;
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

float Scene::getCutterSpeed() const
{
	return m_activeCutter->getSpeed();
}

void Scene::setCutterSpeed(float speed)
{
	m_activeCutter->setSpeed(speed);
}

bool Scene::getRenderToolpath() const
{
	return m_renderToolpath;
}

void Scene::setRenderToolpath(bool renderToolpath)
{
	m_renderToolpath = renderToolpath;
}

std::string& Scene::getWarnings()
{
	return m_warnings;
}
