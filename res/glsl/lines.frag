#version 330

uniform vec3 uColor;

out vec4 Result;

void main()
{
	Result = vec4(uColor,1.f);
}