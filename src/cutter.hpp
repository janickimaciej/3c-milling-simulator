#pragma once

#include "mesh.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <string>

class Cutter
{
public:
	Cutter(const std::string& meshPath);
	void render() const;
	void setRadius(float radius);
	void setMillingHeight(float millingHeight);
	void setPos(const glm::vec3& pos);

private:
	float m_radius = 10;
	float m_millingHeight = 20;
	glm::vec3 m_pos{0, 100, 0};
	glm::mat4 m_modelMatrix{};

	std::unique_ptr<Mesh> m_mesh{};

	void updateModelMatrix();
};
