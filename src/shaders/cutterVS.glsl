#version 420 core

layout (location = 0) in vec3 inPosMesh;
layout (location = 1) in vec3 inNormalVec;

uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;

out vec3 pos;
out vec3 normalVec;

void main()
{
	pos = (modelMatrix * vec4(inPosMesh, 1)).xyz;
	normalVec = normalize(modelMatrix * vec4(inNormalVec, 0)).xyz;
	gl_Position = projectionViewMatrix * vec4(pos, 1);
}
