#version 330

layout(location = 0) in vec3 position;

uniform mat4 camera_mat;

void main()
{
	gl_Position = camera_mat * vec4(position,1.f);
}