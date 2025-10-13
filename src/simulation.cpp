#include "simulation.hpp"

#include <limits>

Simulation::Simulation(const Toolpath& toolpath) :
	m_toolpath{toolpath}
{ }

void Simulation::start()
{
	m_t = std::chrono::system_clock::now();
	m_running = true;
}

void Simulation::step(float simulationSpeed, const glm::vec3& materialSize,
	const glm::ivec2& gridSize, Surface& surface, Cutter& cutter, Texture& heightMap)
{
	if (!m_running)
	{
		return;
	}

	std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();
	float dt = static_cast<std::chrono::duration<float>>(t - m_t).count() * simulationSpeed;
	m_t = t;

	float dist = cutter.getSpeed() * dt;
	static constexpr float eps = 1e-6f;
	while (dist > eps)
	{
		if (m_segmentIndex == m_toolpath.segmentCount())
		{
			m_running = false;
			m_segmentIndex = 0;
			return;
		}

		Toolpath::Segment segment = m_toolpath.getSegment(m_segmentIndex);
		glm::vec3 start = segment.pos(m_segmentPosRelative);
		glm::vec3 end{};
		float remainingSegmentLength = segment.length(m_segmentPosRelative);
		if (remainingSegmentLength <= dist)
		{
			end = segment.pos(1);
			dist -= remainingSegmentLength;
			++m_segmentIndex;
			m_segmentPosRelative = 0;
		}
		else
		{
			float endRelative = m_segmentPosRelative + dist / segment.length();
			end = segment.pos(endRelative);
			dist = 0;
			m_segmentPosRelative = endRelative;
		}
		cutter.setPos(end);

		millSegment(materialSize, gridSize, surface, cutter, heightMap,
			Toolpath::Segment{start, end});
	}
}

void Simulation::stop()
{
	m_running = false;
}

void Simulation::millInstantly(const glm::vec3& materialSize, const glm::ivec2& gridSize,
	Surface& surface, const Cutter& cutter, Texture& heightMap)
{
	for (int segment = 0; segment < m_toolpath.segmentCount(); ++segment)
	{
		millSegment(materialSize, gridSize, surface, cutter, heightMap,
			m_toolpath.getSegment(segment));
	}
}

void Simulation::millSegment(const glm::vec3& materialSize, const glm::ivec2& gridSize,
	Surface& surface, const Cutter& cutter, Texture& heightMap, const Toolpath::Segment& segment)
{
	glm::ivec2 minMilledGrid = gridSize;
	glm::ivec2 maxMilledGrid{0, 0};

	glm::vec3 pos1 = segment.pos(0);
	glm::vec3 pos2 = segment.pos(1);
	glm::vec2 pos1Grid = posToGridPos(materialSize, gridSize, pos1);
	glm::vec2 pos2Grid = posToGridPos(materialSize, gridSize, pos2);

	float dX = pos2.x - pos1.x;
	float dZ = pos2.z - pos1.z;
	float dXGrid = pos2Grid.x - pos1Grid.x;
	float dYGrid = pos2Grid.y - pos1Grid.y;

	if (std::abs(dXGrid) > std::abs(dYGrid))
	{
		float width = cutter.getDiameter() * std::sqrt(1 + std::pow(dZ / dX, 2.0f));
		float widthGrid = width / materialSize.z * gridSize.y;

		float a = dYGrid / dXGrid;
		float b = (pos2Grid.x * pos1Grid.y - pos1Grid.x * pos2Grid.y) / dXGrid;
		float bMin = b - widthGrid / 2.0f;
		float bMax = b + widthGrid / 2.0f;

		float xGridMin = std::min(pos1Grid.x, pos2Grid.x) -
			cutter.getDiameter() / 2.0f / materialSize.x * gridSize.x;
		float xGridMax = std::max(pos1Grid.x, pos2Grid.x) +
			cutter.getDiameter() / 2.0f / materialSize.x * gridSize.x;

		int xStart = std::max(static_cast<int>(std::ceil(xGridMin)), 0);
		int xEnd = std::min(static_cast<int>(std::floor(xGridMax)), gridSize.x);
		for (int x = xStart; x <= xEnd; ++x)
		{
			int yStart = std::max(static_cast<int>(std::ceil(a * x + bMin)), 0);
			int yEnd = std::min(static_cast<int>(std::floor(a * x + bMax)), gridSize.y);
			for (int y = yStart; y <= yEnd; ++y)
			{
				if (!isPointInsideMillProjection(gridPosToPos(materialSize, gridSize, {x, y}),
					cutter, segment))
				{
					continue;
				}

				if (millPoint(materialSize, gridSize, surface, cutter, segment, {x, y}))
				{
					minMilledGrid.x = std::min(minMilledGrid.x, x);
					maxMilledGrid.x = std::max(maxMilledGrid.x, x);
					minMilledGrid.y = std::min(minMilledGrid.y, y);
					maxMilledGrid.y = std::max(maxMilledGrid.y, y);
				}
			}
		}
	}
	else
	{
		float width = cutter.getDiameter() * std::sqrt(1 + std::pow(dX / dZ, 2.0f));
		float widthGrid = width / materialSize.x * gridSize.x;

		float a = dXGrid / dYGrid;
		float b = (pos2Grid.y * pos1Grid.x - pos1Grid.y * pos2Grid.x) / dYGrid;
		float bMin = b - widthGrid / 2.0f;
		float bMax = b + widthGrid / 2.0f;

		float yGridMin = std::min(pos1Grid.y, pos2Grid.y) -
			cutter.getDiameter() / 2.0f / materialSize.z * gridSize.y;
		float yGridMax = std::max(pos1Grid.y, pos2Grid.y) +
			cutter.getDiameter() / 2.0f / materialSize.z * gridSize.y;

		int yStart = std::max(static_cast<int>(std::ceil(yGridMin)), 0);
		int yEnd = std::min(static_cast<int>(std::floor(yGridMax)), gridSize.y);
		for (int y = yStart; y <= yEnd; ++y)
		{
			int xStart = std::max(static_cast<int>(std::ceil(a * y + bMin)), 0);
			int xEnd = std::min(static_cast<int>(std::floor(a * y + bMax)), gridSize.x);
			for (int x = xStart; x <= xEnd; ++x)
			{
				if (!isPointInsideMillProjection(gridPosToPos(materialSize, gridSize, {x, y}),
					cutter, segment))
				{
					continue;
				}

				if (millPoint(materialSize, gridSize, surface, cutter, segment, {x, y}))
				{
					minMilledGrid.x = std::min(minMilledGrid.x, x);
					maxMilledGrid.x = std::max(maxMilledGrid.x, x);
					minMilledGrid.y = std::min(minMilledGrid.y, y);
					maxMilledGrid.y = std::max(maxMilledGrid.y, y);
				}
			}
		}
	}

	if (minMilledGrid.x <= maxMilledGrid.x && minMilledGrid.y <= maxMilledGrid.y)
	{
		heightMap.update(minMilledGrid.x, minMilledGrid.y, maxMilledGrid.x - minMilledGrid.x + 1,
			maxMilledGrid.y - minMilledGrid.y + 1, surface);
	}
}

bool Simulation::millPoint(const glm::vec3& materialSize, const glm::ivec2& gridSize,
	Surface& surface, const Cutter& cutter, const Toolpath::Segment& segment,
	const glm::ivec2& gridPos)
{
	if (cutter.type() == CutterType::flat)
	{
		if (cutter.getPos().y >= surface[gridPos.x][gridPos.y])
		{
			return false;
		}
		surface[gridPos.x][gridPos.y] = cutter.getPos().y;
		return true;
	}

	glm::vec3 pos = gridPosToPos(materialSize, gridSize, gridPos);
	glm::vec3 start = segment.pos(0);
	glm::vec3 end = segment.pos(1);
	pos.y = surface[gridPos.x][gridPos.y];

	float cutterHeight = getMillSphereHeight(cutter, start, pos);
	cutterHeight = std::min(cutterHeight, getMillCylinderHeight(cutter, start, end, pos));
	cutterHeight = std::min(cutterHeight, getMillSphereHeight(cutter, end, pos));

	if (cutterHeight >= surface[gridPos.x][gridPos.y])
	{
		return false;
	}
	surface[gridPos.x][gridPos.y] = cutterHeight;
	return true;
}

bool Simulation::isPointInsideMillProjection(glm::vec3 point, const Cutter& cutter,
	const Toolpath::Segment& segment)
{
	point.y = 0;
	glm::vec3 start = segment.pos(0);
	start.y = 0;
	glm::vec3 end = segment.pos(1);
	end.y = 0;
	glm::vec3 segmentVec = end - start;
	glm::vec3 startToPointVec = point - start;
	glm::vec3 endToPointVec = point - end;

	if (glm::dot(startToPointVec, segmentVec) < 0)
	{
		return glm::dot(startToPointVec, startToPointVec) <
			std::pow(cutter.getDiameter() / 2.0f, 2.0f);
	}
	else if (glm::dot(endToPointVec, segmentVec) > 0)
	{
		return glm::dot(endToPointVec, endToPointVec) <
			std::pow(cutter.getDiameter() / 2.0f, 2.0f);
	}
	return true;
}

float Simulation::getMillSphereHeight(const Cutter& cutter, const glm::vec3& center,
	const glm::vec3& pos)
{
	float sqrtArg = std::pow(cutter.getDiameter() / 2.0f, 2.0f) -
		std::pow(pos.x - center.x, 2.0f) - std::pow(pos.z - center.z, 2.0f);

	if (sqrtArg < 0)
	{
		return std::numeric_limits<float>::max();
	}
	return center.y - std::sqrt(sqrtArg);
}

float Simulation::getMillCylinderHeight(const Cutter& cutter, const glm::vec3& start,
	const glm::vec3& end, const glm::vec3& pos)
{
	glm::vec3 v = end - start;
	v = glm::normalize(v);
	float dx = pos.x - start.x;
	float dz = pos.z - start.z;
	float dx2 = std::pow(dx, 2.0f);
	float dz2 = std::pow(dz, 2.0f);

	float a = 1 - std::pow(v.y, 2.0f);
	static constexpr float eps = 1e-6f;
	if (a < eps)
	{
		return std::numeric_limits<float>::max();
	}
	float b = -2 * (dx * v.x * v.y + v.y * dz * v.z);
	float c = dx2 + dz2 - dx2 * std::pow(v.x, 2.0f) - dz2 * std::pow(v.z, 2.0f) -
		2 * dz * v.z * dx * v.x - std::pow(cutter.getDiameter() / 2.0f, 2.0f);
	float sqrtArg = std::pow(b, 2.0f) - 4 * a * c;

	if (sqrtArg < 0)
	{
		return std::numeric_limits<float>::max();
	}

	glm::vec3 millSurfacePos = pos;
	millSurfacePos.y = start.y + (-b - std::sqrt(sqrtArg)) / (2 * a);
	if (glm::dot(millSurfacePos - start, v) > 0 && glm::dot(millSurfacePos - end, v) < 0)
	{
		return millSurfacePos.y;
	}

	millSurfacePos.y = start.y + (-b + std::sqrt(sqrtArg)) / (2 * a);
	if (glm::dot(millSurfacePos - start, v) > 0 && glm::dot(millSurfacePos - end, v) < 0)
	{
		return millSurfacePos.y;
	}

	return std::numeric_limits<float>::max();
}

glm::vec3 Simulation::gridPosToPos(const glm::vec3& materialSize, const glm::ivec2& gridSize,
	const glm::vec2& gridPos)
{
	return {(gridPos.x / gridSize.x - 0.5f) * materialSize.x, 0,
		-(gridPos.y / gridSize.y - 0.5f) * materialSize.z};
}

glm::vec2 Simulation::posToGridPos(const glm::vec3& materialSize, const glm::ivec2& gridSize,
	const glm::vec3& pos)
{
	return {(pos.x / materialSize.x + 0.5f) * gridSize.x,
		(-pos.z / materialSize.z + 0.5f) * gridSize.y};
}
