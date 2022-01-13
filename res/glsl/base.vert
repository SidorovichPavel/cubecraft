#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;

out vec3 fColor;

uniform mat4 transform;

void main()
{
	fColor = vColor;
	gl_Position = transform * vec4(vPosition, 1.f);
}