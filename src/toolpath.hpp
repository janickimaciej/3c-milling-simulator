#pragma once

#include <glm/glm.hpp>

#include <utility>
#include <vector>

class Toolpath
{
public:
	class Segment
	{
	public:
		Segment(const glm::vec3& start, const glm::vec3& end);
		glm::vec3 pos(float relative) const;
		float length(float startRelative = 0, float endRelative = 1) const;

	private:
		glm::vec3 m_start{};
		glm::vec3 m_end{};
	};

	Toolpath(const std::vector<glm::vec3>& poss);
	int segmentCount() const;
	Segment getSegment(int index) const;
	std::vector<glm::vec3> getPoss() const;

private:
	std::vector<glm::vec3> m_poss{};
};
