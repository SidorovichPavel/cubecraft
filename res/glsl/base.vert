#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

out vec3 fNormal;
out vec3 fPosition;

uniform mat4 world;
uniform mat4 model;

void main()
{
	fPosition = vec3(model * vec4(vPosition, 1.f));
	fNormal = vNormal;

	gl_Position = world * model * vec4(vPosition, 1.f);
}