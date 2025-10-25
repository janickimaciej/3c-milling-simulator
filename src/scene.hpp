#pragma once

#include "camera.hpp"
#include "cutters/cutter.hpp"
#include "cutters/cutterType.hpp"
#include "cutters/flatCutter.hpp"
#include "cutters/roundCutter.hpp"
#include "meshes/faceMesh.hpp"
#include "meshes/polylineMesh.hpp"
#include "shaderProgram.hpp"
#include "simulation.hpp"
#include "surface.hpp"
#include "texture.hpp"
#include "toolpath.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <string>

class Scene
{
public:
	Scene(const glm::ivec2& windowSize);

	void update();
	void render();
	void updateWindowSize();

	void loadToolpathsFile(const std::string& path);
	void mill();
	void stop();
	void millInstantly();
	void reset();

	void moveXCamera(float x);
	void moveYCamera(float y);
	void addPitchCamera(float pitchRad);
	void addYawCamera(float yawRad);
	void zoomCamera(float zoom);

	float getSimulationSpeed() const;
	void setSimulationSpeed(float simulationSpeed);
	glm::vec3 getMaterialSize() const;
	void setMaterialSize(const glm::vec3& materialSize);
	glm::ivec2 getGridSize() const;
	void setGridSize(const glm::ivec2& gridSize);
	float getBaseY() const;
	void setBaseY(float baseY);
	CutterType getCutterType() const;
	void setCutterType(CutterType cutterType);
	float getCutterDiameter() const;
	void setCutterDiameter(float diameter);
	float getCutterMillingHeight() const;
	void setCutterMillingHeight(float millingHeight);
	float getCutterSpeed() const;
	void setCutterSpeed(float speed);
	bool getRenderToolpath() const;
	void setRenderToolpath(bool renderToolpath);
	std::string& getWarnings();

private:
	Camera m_camera;

	float m_simulationSpeed = 1;
	glm::vec3 m_materialSize{150, 50, 180};
	glm::ivec2 m_gridSize{300, 360};
	float m_baseY = 15;

	Surface m_surface;
	Texture m_heightMap;
	FaceMesh m_faceMesh{};

	RoundCutter m_roundCutter{};
	FlatCutter m_flatCutter{};
	Cutter* m_activeCutter{};

	std::unique_ptr<Toolpath> m_toolpath = nullptr;
	std::unique_ptr<PolylineMesh> m_toolpathMesh = nullptr;
	bool m_renderToolpath = false;
	std::unique_ptr<Simulation> m_simulation = nullptr;

	std::string m_warnings = "";
};
