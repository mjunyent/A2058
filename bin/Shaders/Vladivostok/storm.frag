#version 330 core

// Ouput data
layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 glowMap;

uniform sampler2D	tex;
uniform float cellPos;

in VertexData {
	vec2 UV;
} VertexIn;

void main()
{
	if(gl_FragCoord.x > cellPos) discard;
	colour = texture(tex, VertexIn.UV); //Get the color from the texture
	glowMap = vec4(0.0, 0.0, 0.0, colour.a);
	if(colour.a < 0.01) discard;
}