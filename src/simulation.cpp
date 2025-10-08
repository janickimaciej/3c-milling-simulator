#include "simulation.hpp"

Simulation::Simulation(const Toolpath& toolpath) :
	m_toolpath{toolpath}
{ }

void Simulation::start()
{
	m_t = std::chrono::system_clock::now();
	m_running = true;
}

void Simulation::step(const glm::vec3& materialSize, const glm::ivec2& gridSize, Surface& surface,
	Cutter& cutter, Texture& heightMap)
{
	if (!m_running)
	{
		return;
	}

	std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();
	float dt = static_cast<std::chrono::duration<float>>(t - m_t).count();
	m_t = t;

	float dist = cutter.getSpeed() * dt;
	while (dist > 0)
	{
		if (m_segmentIndex == m_toolpath.segmentCount())
		{
			m_running = false;
			m_segmentIndex = 0;
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

void Simulation::millSegment(const glm::vec3& materialSize, const glm::ivec2& gridSize,
	Surface& surface, const Cutter& cutter, Texture& heightMap, const Toolpath::Segment& segment)
{
	glm::vec2 relativeMin{1.0f, 1.0f};
	glm::vec2 relativeMax{0.0f, 0.0f};

	glm::vec3 pos1 = segment.pos(0);
	glm::vec3 pos2 = segment.pos(1);
	glm::vec2 pos1Relative = posToRelativePos(materialSize, pos1);
	glm::vec2 pos2Relative = posToRelativePos(materialSize, pos2);

	float dX = pos2.x - pos1.x;
	float dZ = pos2.z - pos1.z;
	float dXRelative = pos2Relative.x - pos1Relative.x;
	float dYRelative = pos2Relative.y - pos1Relative.y;

	if (dXRelative > dYRelative)
	{
		float width = cutter.getDiameter() * std::sqrt(1 + std::pow(dZ / dX, 2.0f));
		float widthRelative = width / materialSize.z;

		float a = dYRelative / dXRelative;
		float b = (pos2Relative.x * pos1Relative.y - pos1Relative.x * pos2Relative.y) / dXRelative;
		float bMin = b - widthRelative / 2.0f;
		float bMax = b + widthRelative / 2.0f;

		float xRelativeMin = (pos1Relative.x < pos2Relative.x ? pos1Relative.x : pos2Relative.x) -
			cutter.getDiameter() / materialSize.x;
		float xRelativeMax = (pos1Relative.x > pos2Relative.x ? pos1Relative.x : pos2Relative.x) +
			cutter.getDiameter() / materialSize.x;

		// TODO: fix x and y, scale with gridSize; check if they don't exceed gridSize
		for (int x = std::ceil(xRelativeMin); x <= std::floor(xRelativeMax); ++x)
		{
			for (int y = std::ceil(a * x + bMin); y <= std::floor(a * x + bMax); ++y)
			{

			}
		}
	}
	else
	{
		float width = cutter.getDiameter() * std::sqrt(1 + std::pow(dX / dZ, 2.0f));
		float widthRelative = width / materialSize.x;

		float a = dYRelative / dXRelative;
		float b = (pos2Relative.x * pos1Relative.y - pos1Relative.x * pos2Relative.y) / dXRelative;
		float bMin = b - widthRelative / 2.0f;
		float bMax = b + widthRelative / 2.0f;

		float yRelativeMin = (pos1Relative.y < pos2Relative.y ? pos1Relative.y : pos2Relative.y) -
			cutter.getDiameter() / materialSize.z;
		float yRelativeMax = (pos1Relative.y > pos2Relative.y ? pos1Relative.y : pos2Relative.y) +
			cutter.getDiameter() / materialSize.z;

		for (int y = std::ceil(yRelativeMin); y <= std::floor(yRelativeMax); ++y)
		{
			for (int x = std::ceil(a * y + bMin); x <= std::floor(a * y + bMax); ++x)
			{

			}
		}
	}
}

glm::vec3 Simulation::relativePosToPos(const glm::vec3& materialSize, const glm::vec2& relativePos)
{
	return {(relativePos.x - 0.5f) * materialSize.x, 0, -(relativePos.y - 0.5f) * materialSize.z};
}

glm::vec2 Simulation::posToRelativePos(const glm::vec3& materialSize, const glm::vec3& pos)
{
	return {pos.x / materialSize.x + 0.5f, -pos.z / materialSize.z + 0.5f};
}
