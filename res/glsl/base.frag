#version 330

in vec3 fColor;
out vec4 fResult;


void main()
{
	fResult = vec4(fColor, 1.f);
}