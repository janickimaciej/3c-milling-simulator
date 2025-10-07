#pragma once

#include "cutters/cutter.hpp"
#include "toolpath.hpp"

#include <chrono>

class Simulation
{
public:
	Simulation(const Toolpath& toolpath);
	void start();
	void step(const Cutter& cutter);
	void stop();

private:
	const Toolpath& m_toolpath;
	bool m_running = false;
	std::chrono::time_point<std::chrono::system_clock> m_t{};
	int m_segmentIndex = 0;
	float m_segmentPosRelative = 0;
};
