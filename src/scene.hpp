#pragma once

#include "camera.hpp"
#include "cutters/cutter.hpp"
#include "cutters/cutterType.hpp"
#include "cutters/flatCutter.hpp"
#include "cutters/roundCutter.hpp"
#include "faceMesh.hpp"
#include "shaderProgram.hpp"
#include "surface.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>

#include <memory>

class Scene
{
public:
	Scene(const glm::ivec2& windowSize);

	void update();
	void render();
	void updateWindowSize();

	void loadPathsFile(const std::string& path);
	void mill();
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
	CutterType getCutterType() const;
	void setCutterType(CutterType cutterType);
	float getCutterDiameter() const;
	void setCutterDiameter(float diameter);
	float getCutterMillingHeight() const;
	void setCutterMillingHeight(float millingHeight);
	float getMaxMillingDepth() const;
	void setMaxMillingDepth(float maxMillingDepth);

private:
	Camera m_camera;

	float m_simulationSpeed = 1;
	glm::vec3 m_materialSize{400, 100, 200};
	glm::ivec2 m_gridSize{20, 10};

	Surface m_surface;
	Texture m_heightMap;
	FaceMesh m_faceMesh{};

	FlatCutter m_flatCutter{};
	RoundCutter m_roundCutter{};

	CutterType m_cutterType{};
	Cutter* m_activeCutter{};
};
