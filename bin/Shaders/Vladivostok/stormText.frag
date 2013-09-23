#version 330 core

// Ouput data
layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 glowMap;

uniform float screenPosition;
uniform sampler2D image;
uniform int show; //if show=0, right of grid is show, the contrary if show=1.

in vec2 UV;

void main()
{
/*	if(UV.x <= 0.01 || UV.x >= 0.99 ||
       UV.y <= 0.01 || UV.y >= 0.99) discard;	*/
	if(show == 0 && gl_FragCoord.x < screenPosition) discard;
	if(show == 1 && gl_FragCoord.x > screenPosition) discard;
	colour = texture(image, vec2(UV.x,1.0-UV.y));
	if(colour.a == 0.0) discard;
	glowMap = vec4(0.0, 0.0, 0.0, 1.0);
}