#pragma once

#include "mesh.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <string>

class CutterElement
{
public:
	CutterElement(const std::string& path);
	void render() const;
	void setScale(const glm::vec3& scale);
	void setPos(const glm::vec3& pos);

private:
	glm::vec3 m_pos{};
	glm::vec3 m_scale{};
	glm::mat4 m_modelMatrix{};

	std::unique_ptr<Mesh> m_mesh{};

	void updateModelMatrix();
};
