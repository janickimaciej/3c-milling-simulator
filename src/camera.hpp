#pragma once

#include "shaderProgram.hpp"

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(const glm::ivec2& windowSize, float fovYDeg, float nearPlane, float farPlane,
		const ShaderProgram& surfaceShaderProgram);
	virtual ~Camera() = default;
	void use() const;
	glm::mat4 getMatrix() const;
	void updateWindowSize();
	float getFOVYDeg() const;
	void setFOVYDeg(float fovYDeg);
	
	void moveX(float x);
	void moveY(float y);
	void addPitch(float pitchRad);
	void addYaw(float yawRad);
	void zoom(float zoom);

private:
	const glm::ivec2& m_windowSize{};
	float m_nearPlane{};
	float m_farPlane{};
	float m_fovYDeg{};

	glm::vec3 m_targetPos{0, 0, 0};
	float m_pitchRad = 0;
	float m_yawRad = 0;
	float m_radius = 3;

	glm::mat4 m_viewMatrixInverse{1};
	glm::mat4 m_projectionMatrix{1};

	const ShaderProgram& m_surfaceShaderProgram;

	glm::vec3 getPos() const;

	void updateViewMatrix();
	void updateProjectionMatrix();
	void updateShaders() const;
};
