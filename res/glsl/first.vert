#version 330

in vec3 position;
in vec3 color;

uniform mat4 transform;

out vec3 pass_color;

void main()
{
	color.xyz;
	pass_color = color;
	gl_Position = transform * vec4(position, 1.f);
}