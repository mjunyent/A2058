#version 330 core

// Ouput data
layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 glowMap;

uniform float screenPosition;
uniform sampler2D leftTex, rightTex;
uniform bool showLeft, showRight; //if show=0, right of grid is show, the contrary if show=1.

in vec2 UV;

void main()
{
/*	if(UV.x <= 0.01 || UV.x >= 0.99 ||
       UV.y <= 0.01 || UV.y >= 0.99) discard;	*/
	if(gl_FragCoord.x < screenPosition) { //We're on the LEFT
		if(!showLeft) discard;
		else {
			colour = texture(leftTex, vec2(UV.x,1.0-UV.y));
		}
	} else if(gl_FragCoord.x > screenPosition) {
		if(!showRight) discard;
		else {
			colour = texture(rightTex, vec2(UV.x,1.0-UV.y));
		}
	} else {
		discard;
	}

	if(colour.a == 0.0) discard;
	glowMap = vec4(0.0, 0.0, 0.0, 1.0);
}