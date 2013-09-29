#version 330 core

// Ouput data
layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 glowMap;

uniform sampler2D	tex;
uniform float cellPos;
uniform float alpha;
uniform int side; //1 = show right, 2 = show left, 0 = show all, 3 = show none

in VertexData {
	vec2 UV;
} VertexIn;

void main()
{
	if(side == 3) discard;
	if(side == 1 && gl_FragCoord.x > cellPos) discard;
	if(side == 2 && gl_FragCoord.x < cellPos) discard;
	colour = texture(tex, VertexIn.UV); //Get the color from the texture
	colour.a -= alpha;
/*	colour.r = colour.r/3.0+colour.g/3.0+colour.b/3.0;
	colour.g = colour.r;
	colour.b = colour.r;*/
	clamp(colour, 0.0, 1.0);

	glowMap = vec4(0.0, 0.0, 0.0, colour.a);
	if(colour.a < 0.01) discard;
}