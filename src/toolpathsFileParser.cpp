#include "toolpathsFileParser.hpp"

#include <cstddef>
#include <fstream>
#include <iostream>

std::vector<glm::vec3> ToolpathsFileParser::parse(const std::string& path)
{
	std::ifstream file{path};
	if (!file)
	{
		std::cerr << "File does not exist:\n" << path << '\n';
		return {};
	}

	std::vector<glm::vec3> poss{};

	std::string line{};
	while (std::getline(file, line))
	{
		if (line[0] != 'N')
		{
			continue;
		}

		std::size_t i = 1;
		for (; isDigit(line[i]); ++i);

		if (i == 1 || i + 2 >= line.size() || line[i] != 'G' || line[i + 1] != '0' ||
			line[i + 2] != '1')
		{
			continue;
		}

		glm::vec3 pos = parsePos(std::string_view(line).substr(i + 3));
		poss.push_back(pos);
	}

	return poss;
}

glm::vec3 ToolpathsFileParser::parsePos(const std::string_view line)
{
	glm::vec3 pos{};

	int component = 0;
	std::string number = "";
	for (auto c = line.begin() + 1; c != line.end(); ++c)
	{
		if (*c == 'Y' || *c == 'Z')
		{
			pos[component] = std::stof(number);
			++component;
			number = "";
		}
		else
		{
			number.push_back(*c);
		}
	}
	pos[component] = std::stof(number);

	return pos;
}

bool ToolpathsFileParser::isDigit(char c)
{
	return c >= '0' && c <= '9';
}
