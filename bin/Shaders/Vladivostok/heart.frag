#version 330 core

// Ouput data
layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 glowMap;

uniform float screenPosition;
uniform sampler2D Tex;
uniform float alpha;

in vec2 UV;

void main()
{
	if(gl_FragCoord.x > screenPosition) {
		colour = texture(Tex, vec2(UV.x,1.0-UV.y));
	} else {
		discard;
	}
	
	colour.a = clamp(colour.a-alpha, 0.0, 1.0);

	if(colour.a == 0.0) discard;
	glowMap = vec4(0.0, 0.0, 0.0, 1.0);
}