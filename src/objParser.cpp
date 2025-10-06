#include "objParser.hpp"

#include <glm/glm.hpp>

#include <cstddef>
#include <fstream>
#include <iostream>

std::vector<Vertex> ObjParser::parse(const std::string& path)
{
	std::ifstream file{path};
	if (!file)
	{
		std::cerr << "File does not exist:\n" << path << '\n';
		return {};
	}

	std::vector<Vertex> vertices{};

	std::vector<glm::vec3> poss{};
	std::vector<glm::vec3> normalVectors{};

	std::string line{};
	while (std::getline(file, line))
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			poss.push_back(parsePos(line));
		}
		else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
		{
			normalVectors.push_back(parseNormalVec(line));
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			std::array<Vertex, 3> triangle = parseTriangle(line, poss, normalVectors);
			vertices.push_back(triangle[0]);
			vertices.push_back(triangle[1]);
			vertices.push_back(triangle[2]);
		}
	}

	return vertices;
}

glm::vec3 ObjParser::parsePos(const std::string_view line)
{
	glm::vec3 pos{};

	int component = 0;
	std::string number = "";
	for (auto c = line.begin() + 2; c != line.end(); ++c)
	{
		if (*c == ' ')
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

glm::vec3 ObjParser::parseNormalVec(const std::string_view line)
{
	glm::vec3 normalVector{};

	int component = 0;
	std::string number = "";
	for (auto c = line.begin() + 3; c != line.end(); ++c)
	{
		if (*c == ' ')
		{
			normalVector[component] = std::stof(number);
			++component;
			number = "";
		}
		else
		{
			number.push_back(*c);
		}
	}
	normalVector[component] = std::stof(number);

	return normalVector;
}

std::array<Vertex, 3> ObjParser::parseTriangle(const std::string_view line,
	const std::vector<glm::vec3>& poss, const std::vector<glm::vec3>& normalVectors)
{
	std::array<Vertex, 3> triangle;

	std::size_t vertexIndex = 0;
	std::string number = "";
	std::size_t posIndex = 0;
	std::size_t normalVectorIndex = 0;
	for (auto c = line.begin() + 2; c != line.end(); ++c)
	{
		if (*c == ' ')
		{
			normalVectorIndex = static_cast<std::size_t>(std::stoi(number));
			number = "";
			if (vertexIndex < 3)
			{
				triangle[vertexIndex].pos = poss[posIndex - 1];
				triangle[vertexIndex].normalVec = normalVectors[normalVectorIndex - 1];
			}
			++vertexIndex;
		}
		else if (*c == '/')
		{
			posIndex = static_cast<std::size_t>(std::stoi(number));
			number = "";
			++c;
		}
		else
		{
			number.push_back(*c);
		}
	}
	normalVectorIndex = static_cast<std::size_t>(std::stoi(number));
	if (vertexIndex < 3)
	{
		triangle[vertexIndex].pos = poss[posIndex - 1];
		triangle[vertexIndex].normalVec = normalVectors[normalVectorIndex - 1];
	}

	return triangle;
}
