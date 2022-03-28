#version 330

in vec3 fNormal;
in vec3 fPosition;

uniform vec3 uObjectColor;

uniform vec3 uLightPos;
uniform vec3 uLightColor;

out vec4 fResult;

void main()
{
	float amb = .1f;
	vec3 ambient = amb * uLightColor;

	vec3 light_dir = normalize(uLightPos - fPosition);
	float diff = max(dot(fNormal, light_dir), 0.f);
	vec3 diffuse = diff * uLightColor;

	vec3 result = (ambient + diffuse) * uObjectColor;
	fResult = vec4(result, 1.f);
}