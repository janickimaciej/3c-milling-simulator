#pragma once

#include <glm/glm.hpp>

#include <utility>
#include <vector>

class Toolpath
{
public:
	Toolpath(const std::vector<glm::vec3>& poss);

private:
	std::vector<glm::vec3> m_poss{};
};
