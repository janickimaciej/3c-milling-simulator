#version 420 core

#define SIDE_LEFT 0
#define SIDE_RIGHT 1
#define SIDE_BACK 2
#define SIDE_FRONT 3

uniform mat4 projectionViewMatrix;
uniform vec3 materialSize;
uniform ivec2 gridSize;
uniform sampler2D heightMap;
uniform int side;

out vec3 pos;
out vec3 normalVec;

void calcPos();
vec2 relativeToTexture(vec2 relativePos);
vec2 relativeToPos(vec2 relativePos);

void main()
{
	calcPos();

	switch (side)
	{
		case SIDE_LEFT:
			normalVec = vec3(-1, 0, 0);
			break;

		case SIDE_RIGHT:
			normalVec = vec3(1, 0, 0);
			break;

		case SIDE_BACK:
			normalVec = vec3(0, 0, -1);
			break;

		case SIDE_FRONT:
			normalVec = vec3(0, 0, 1);
			break;
	}

	gl_Position = projectionViewMatrix * vec4(pos, 1);
}

void calcPos()
{
	int segmentIndex = gl_VertexID / 6;
	int vertexQuadIndex = gl_VertexID % 6;

	int axisSize;
	if (side == SIDE_LEFT || side == SIDE_RIGHT)
	{
		axisSize = gridSize.y;
	}
	else
	{
		axisSize = gridSize.x;
	}
	
	float axisPos = float(segmentIndex) / axisSize;
	if (vertexQuadIndex == 1 || vertexQuadIndex == 2 || vertexQuadIndex == 4)
	{
		axisPos += 1.0 / axisSize;
	}

	vec2 relativePos;
	switch (side)
	{
		case SIDE_LEFT:
			relativePos.x = 0;
			relativePos.y = 1 - axisPos;
			break;

		case SIDE_RIGHT:
			relativePos.x = 1;
			relativePos.y = axisPos;
			break;

		case SIDE_BACK:
			relativePos.x = 1 - axisPos;
			relativePos.y = 1;
			break;

		case SIDE_FRONT:
			relativePos.x = axisPos;
			relativePos.y = 0;
			break;
	}

	pos.xz = relativeToPos(relativePos);
	if (vertexQuadIndex == 0 || vertexQuadIndex == 1 || vertexQuadIndex == 3)
	{
		pos.y = -materialSize.y;
	}
	else
	{
		pos.y = texture(heightMap, relativeToTexture(relativePos)).r;
	}
}

vec2 relativeToTexture(vec2 relativePos)
{
	return (0.5 + gridSize * relativePos) / (gridSize + 1);
}

vec2 relativeToPos(vec2 relativePos)
{
	return vec2((relativePos.x - 0.5) * materialSize.x, -(relativePos.y - 0.5) * materialSize.z);
}
