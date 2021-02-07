#version 330

in vec3 position;
in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

out vec3 pass_color;

void main()
{
	pass_color = color;
	gl_Position = perspective * view * model * vec4(position, 1.f);
}