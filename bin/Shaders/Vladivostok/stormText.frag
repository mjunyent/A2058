#version 330 core

// Ouput data
layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 glowMap;

uniform float screenPosition;
uniform sampler2D image;

in vec2 UV;

void main()
{
	if(gl_FragCoord.x < screenPosition) discard;
	colour = texture(image, vec2(UV.x,-UV.y));
	if(colour.a == 0.0) discard;
//	colour = vec4(1.0, abs(UV.xy), 1.0);
	glowMap = vec4(0.0, 0.0, 0.0, 1.0);
}