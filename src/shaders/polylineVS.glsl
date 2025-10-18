#version 420 core

layout (location = 0) in vec3 inPosMesh;

uniform mat4 projectionViewMatrix;

void main()
{
	gl_Position = projectionViewMatrix * vec4(inPosMesh, 1);
}
