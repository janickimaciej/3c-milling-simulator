#include "toolpath.hpp"

Toolpath::Segment::Segment(const glm::vec3& start, const glm::vec3& end) :
	m_start{start},
	m_end{end}
{ }

glm::vec3 Toolpath::Segment::pos(float relative) const
{
	return m_start + relative * (m_end - m_start);
}

float Toolpath::Segment::length(float startRelative, float endRelative) const
{
	return glm::length(pos(endRelative) - pos(startRelative));
}

Toolpath::Toolpath(const std::vector<glm::vec3>& poss) :
	m_poss{poss}
{ }

int Toolpath::segmentCount() const
{
	return static_cast<int>(m_poss.size());
}

Toolpath::Segment Toolpath::getSegment(int index) const
{
	return {m_poss[index], m_poss[index + 1]};
}
