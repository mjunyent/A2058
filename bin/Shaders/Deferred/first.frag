#version 330 core

// Ouput data
layout(location = 0) out vec4 normal_c;
layout(location = 1) out vec4 difuse;
layout(location = 2) out vec4 specular;

in vec4 normal;
in vec4 position;

void main()
{
	float shade = dot(normalize(normal.xyz), vec3(0, 1, 0)); 
	specular = vec4(0.2+0.6*abs(shade), 0.2+0.6*abs(shade), 0.2+0.6*abs(shade), 1);

	normal_c = vec4(normal.xyz, 1.0);
	difuse = vec4(position.xyz, 1.0);
	//specular = 	vec4(0, 0, 1, 1);
}