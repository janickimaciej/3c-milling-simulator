#version 420 core

in vec3 pos;
in vec3 normalVec;

uniform vec3 cameraPos;

out vec4 outColor;

void main()
{
	vec3 color = vec3(1, 1, 1);
	const float ambient = 0.2f;
	const float diffuse = 0.4f;
	const float specular = 0.4f;
	const float shininess = 20.0f;

	float brightness = ambient;

	vec3 normalizedNormalVec = normalize(normalVec);
	vec3 viewVec = normalize(cameraPos - pos);
	vec3 lightVec = viewVec;

	float lightNormalCos = dot(lightVec, normalizedNormalVec);
	brightness += lightNormalCos > 0 ? diffuse * lightNormalCos : 0;

	vec3 reflectionVec = 2 * dot(lightVec, normalizedNormalVec) * normalizedNormalVec - lightVec;
	float reflectionViewCos = dot(reflectionVec, viewVec);
	brightness += reflectionViewCos > 0 ? specular * pow(reflectionViewCos, shininess) : 0;

	outColor = vec4(brightness * color, 1);
}
