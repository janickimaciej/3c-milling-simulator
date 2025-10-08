#version 420 core

uniform mat4 projectionViewMatrix;
uniform vec3 materialSize;

out vec3 pos;
out vec3 normalVec;

vec2 relativeToPos(vec2 relativePos);

void main()
{
	vec2 relativePos[6] = {vec2(0, 0), vec2(1, 1), vec2(1, 0), vec2(0, 0), vec2(0, 1), vec2(1, 1)};
	pos.xz = relativeToPos(relativePos[gl_VertexID]);
	pos.y = 0;
	normalVec = vec3(0, -1, 0);

	gl_Position = projectionViewMatrix * vec4(pos, 1);
}

vec2 relativeToPos(vec2 relativePos)
{
	return vec2((relativePos.x - 0.5) * materialSize.x, -(relativePos.y - 0.5) * materialSize.z);
}
