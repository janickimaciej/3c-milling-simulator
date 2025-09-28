#version 420 core

uniform mat4 projectionViewMatrix;
uniform vec2 materialSize;
uniform ivec2 gridSize;
uniform sampler2D heightMap;

out vec3 pos;
out vec3 normalVec;

vec3[3] getTriangle();
vec2 getTexturePos(vec2 relativePos);
vec2 getPos(vec2 relativePos);

void main()
{
	vec3 triangle[3] = getTriangle();
	pos = triangle[gl_VertexID % 3];
	normalVec = normalize(cross(triangle[1] - triangle[0], triangle[2] - triangle[0]));

	gl_Position = projectionViewMatrix * vec4(pos, 1);
}

vec3[3] getTriangle()
{
	int quadIndex = gl_VertexID / 6;
	int quadX = quadIndex % gridSize.x;
	int quadY = quadIndex / gridSize.x;
	vec2 quadPos = vec2(float(quadX) / gridSize.x, float(quadY) / gridSize.y);

	float dx = 1.0 / gridSize.x;
	float dy = 1.0 / gridSize.y;
	int vertexQuadIndex = gl_VertexID % 6;

	vec2 triangleRelative[3];
	triangleRelative[0] = quadPos;
	if (vertexQuadIndex < 3)
	{
		triangleRelative[1] = vec2(quadPos.x + dx, quadPos.y);
		triangleRelative[2] = vec2(quadPos.x + dx, quadPos.y + dy);
	}
	else
	{
		triangleRelative[1] = vec2(quadPos.x + dx, quadPos.y + dy);
		triangleRelative[2] = vec2(quadPos.x, quadPos.y + dy);
	}

	vec3 triangle[3];
	for (int i = 0; i < 3; ++i)
	{
		triangle[i].xz = getPos(triangleRelative[i]);
		triangle[i].y = texture(heightMap, getTexturePos(triangleRelative[i])).r;
	}

	return triangle;
}

vec2 getTexturePos(vec2 relativePos)
{
	return (0.5 + gridSize * relativePos) / (gridSize + 1);
}

vec2 getPos(vec2 relativePos)
{
	return vec2((relativePos.x - 0.5) * materialSize.x, -(relativePos.y - 0.5) * materialSize.y);
}
