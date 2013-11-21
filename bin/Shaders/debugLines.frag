#version 330 core

// Ouput data
out vec4 colour;

uniform vec3 Color;

void main()
{
	// Output color = red 
	colour = vec4(Color.rgb, 1.0);
}