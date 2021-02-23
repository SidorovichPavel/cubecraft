#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 uv;

out vec3 v_pos;
out vec3 v_norm;
out vec2 v_uv;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(position,1.f);
	v_pos = position;
	v_norm = norm;
	v_uv = uv;
}