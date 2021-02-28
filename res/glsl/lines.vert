#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 camera_mat4;

out vec4 v_Color;

void main()
{
	gl_Position = camera_mat4 * vec4(position,1.f);
	v_Color = color;
}