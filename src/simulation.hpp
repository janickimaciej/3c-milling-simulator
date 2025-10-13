#pragma once

#include "cutters/cutter.hpp"
#include "surface.hpp"
#include "texture.hpp"
#include "toolpath.hpp"

#include <chrono>

class Simulation
{
public:
	Simulation(const Toolpath& toolpath);
	void start();
	void step(float simulationSpeed, const glm::vec3& materialSize, const glm::ivec2& gridSize,
		Surface& surface, Cutter& cutter, Texture& heightMap);
	void stop();
	void millInstantly(const glm::vec3& materialSize,
		const glm::ivec2& gridSize, Surface& surface, const Cutter& cutter, Texture& heightMap);

private:
	const Toolpath& m_toolpath;
	bool m_running = false;
	std::chrono::time_point<std::chrono::system_clock> m_t{};
	int m_segmentIndex = 0;
	float m_segmentPosRelative = 0;

	static void millSegment(const glm::vec3& materialSize, const glm::ivec2& gridSize,
		Surface& surface, const Cutter& cutter, Texture& heightMap,
		const Toolpath::Segment& segment);
	static bool millPoint(const glm::vec3& materialSize, const glm::ivec2& gridSize,
		Surface& surface, const Cutter& cutter, const Toolpath::Segment& segment,
		const glm::ivec2& gridPos);
	static bool isPointInsideMillProjection(glm::vec3 point, const Cutter& cutter,
		const Toolpath::Segment& segment);
	static float getMillSphereHeight(const Cutter& cutter, const glm::vec3& center,
		const glm::vec3& pos);
	static float getMillCylinderHeight(const Cutter& cutter, const glm::vec3& start,
		const glm::vec3& end, const glm::vec3& pos);
	static glm::vec3 gridPosToPos(const glm::vec3& materialSize, const glm::ivec2& gridSize,
		const glm::vec2& gridPos);
	static glm::vec2 posToGridPos(const glm::vec3& materialSize, const glm::ivec2& gridSize,
		const glm::vec3& pos);
};
