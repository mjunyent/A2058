#version 330 core

// Ouput data
layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 glowMap;

uniform sampler2D	tex;

in VertexData {
	vec2 UV;
} VertexIn;

void main()
{
	glowMap = vec4(0.0);
	colour = texture(tex, VertexIn.UV); //Get the color from the texture
	if(colour.a < 0.01) discard;
}