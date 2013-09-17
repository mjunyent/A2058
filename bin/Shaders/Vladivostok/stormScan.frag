#version 330 core

// Ouput data
layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 glowMap;

uniform vec3 centerPosition;
uniform float radius;
in vec4 position;

void main()
{
	colour = vec4(1.0, 1.0, 1.0, 1.0);
	glowMap = vec4(1.0);
	if(length(centerPosition-position.xyz) < radius) discard;
}