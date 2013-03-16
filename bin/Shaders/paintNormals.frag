#version 330 core

// Ouput data
out vec4 color;

in vec4 normal;

void main()
{
	float shade = abs(dot(normal.xyz, vec3(0, 1, 0))); 
	color = vec4(shade*230/255., shade*0/255., shade*38/255., 1);
}