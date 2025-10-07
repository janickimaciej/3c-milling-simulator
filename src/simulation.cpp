#include "simulation.hpp"

Simulation::Simulation(const Toolpath& toolpath) :
	m_toolpath{toolpath}
{ }

void Simulation::start()
{
	m_t = std::chrono::system_clock::now();
	m_running = true;
}

void Simulation::step(const Cutter& cutter)
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

		// TODO: MillingAlg::mill(surface, cutter, texture, Toolpath::Segment{start, end});
	}
}

void Simulation::stop()
{
	m_running = false;
}
