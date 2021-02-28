#version 330

struct Material
{
	sampler2D diffuse;
	sampler2D normal;
	sampler2D specular;
};

uniform Material material;

uniform vec3 light_pos;
uniform vec3 light_color;

in vec3 v_pos;
in vec3 v_norm;
in vec2 v_uv;

out vec4 Result;

#define EPSILON 0.0001f

void main()
{
	
	vec3 obj_color = vec3(texture(material.diffuse, v_uv));

	//ambient
	float ambientStrength = 0.3f;
	vec3 ambient = ambientStrength * light_color;

	//diffuse
	vec3 norm = v_norm;//vec3(texture(material.normal, v_uv));
	//norm = rotate(norm, v_norm, v_angle);
	
	vec3 lightDir = normalize(light_pos - v_pos);
	float diff = max(dot(norm,lightDir),0.f) * max(dot(vec3(0.f,1.f,0.f), lightDir), 0.f);
	vec3 diffuse = diff * light_color; 

	vec3 specular = vec3(texture(material.specular, v_uv)) * light_color;

	vec3 res = (ambient + diffuse + specular) * obj_color;
	
	Result = vec4(res,1.f);

	float specularStrength = 0.5f;
}