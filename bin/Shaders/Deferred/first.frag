#version 330 core

// Ouput data
layout(location = 0) out vec4 normal_c;
layout(location = 1) out vec4 difuse;
layout(location = 2) out vec4 specular;

in vec4 normal;
in vec4 position;

uniform float portillo;

void main()
{	
	normal_c = vec4(normal.xyz, 1.0);
	difuse = vec4(position.xyz, 1.0);
	//specular = 	vec4(0, 0, 1, 1);

	float shade = dot(normalize(normal.xyz), vec3(0, 1, 0));
	specular = vec4(0.0, portillo, 1.0, 1.0);
}