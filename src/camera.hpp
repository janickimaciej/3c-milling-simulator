#pragma once

#include "shaderProgram.hpp"

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(const glm::ivec2& viewportSize, float fovYDeg, float nearPlane, float farPlane);
	virtual ~Camera() = default;
	void use() const;
	glm::mat4 getMatrix() const;
	void updateViewportSize();
	float getFOVYDeg() const;
	void setFOVYDeg(float fovYDeg);

	void moveX(float x);
	void moveY(float y);
	void addPitch(float pitchRad);
	void addYaw(float yawRad);
	void zoom(float zoom);

private:
	const glm::ivec2& m_viewportSize{};
	float m_nearPlane{};
	float m_farPlane{};
	float m_fovYDeg{};

	glm::vec3 m_targetPos{0, 0, 0};
	float m_pitchRad = 0;
	float m_yawRad = 0;
	float m_radius = 500;

	glm::mat4 m_viewMatrixInverse{1};
	glm::mat4 m_projectionMatrix{1};

	glm::vec3 getPos() const;

	void updateViewMatrix();
	void updateProjectionMatrix();
	void updateShaders() const;
};
