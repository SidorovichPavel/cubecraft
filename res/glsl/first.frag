#version 330

in vec3 pass_color;
out vec4 color;

void main()
{
	color = vec4(pass_color, 1.f);
}