#version 330

in vec3 position;
in vec3 color;

out vec3 pass_color;

void main()
{
	pass_color = color;
	gl_Position = vec4(position, 1.f);
}