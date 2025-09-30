#pragma once

#include "vertex.hpp"

#include <glm/glm.hpp>

#include <array>
#include <string>
#include <string_view>
#include <vector>

class ObjParser
{
public:
	ObjParser() = delete;
	static std::vector<Vertex> parse(const std::string& path);
	~ObjParser() = delete;

private:
	static glm::vec3 parsePos(const std::string_view line);
	static glm::vec3 parseNormalVec(const std::string_view line);
	static std::array<Vertex, 3> parseTriangle(const std::string_view line,
		const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normalVectors);
};
