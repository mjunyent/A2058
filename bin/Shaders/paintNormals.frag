#version 330 core

// Ouput data
out vec4 color;

in vec4 normal;

void main()
{
	float shade = abs(dot(normal.xyz, vec3(0, 1, 0))); 
	color = vec4(shade, shade, shade, 1);
	color = vec4(abs(normal.xyz), 1);
}
