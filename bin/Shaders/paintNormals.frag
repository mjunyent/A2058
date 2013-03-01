#version 330 core

// Ouput data
out vec4 color;

varying vec4 normal;

void main()
{
	// Output color = red 
	color = vec4(abs(dot(normal.xyz, vec3(0, 1, 0))), 0, 0, 1);
}