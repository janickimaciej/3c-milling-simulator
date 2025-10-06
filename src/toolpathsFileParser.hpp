#pragma once

#include "toolpath.hpp"

#include <glm/glm.hpp>

#include <string>
#include <vector>

class ToolpathsFileParser
{
public:
	ToolpathsFileParser() = delete;
	static std::vector<glm::vec3> parse(const std::string& path);
	~ToolpathsFileParser() = delete;

private:
	static glm::vec3 parsePos(const std::string_view line);
	static bool isDigit(char c);
};
