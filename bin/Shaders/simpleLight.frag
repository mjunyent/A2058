#version 330 core

// Ouput data
layout(location = 0) out vec4 color;

in vec4 normal;

void main()
{
	float shade = dot(normalize(normal.xyz), vec3(0, 1, 0)); 
	color = vec4(0.2+0.6*abs(shade), 0.2+0.6*abs(shade), 0.2+0.6*abs(shade), 1);
}