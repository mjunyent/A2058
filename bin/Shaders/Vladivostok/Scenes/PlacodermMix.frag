#version 330 core

in vec2 UV;
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 glowMap;
//layout (depth_unchanged) out float gl_FragDepth;

uniform float		position;
uniform float		alphaValue;

uniform sampler2D 	TexIn;
uniform sampler2D	TexOut;

uniform sampler2D	DepthIn;
uniform sampler2D	DepthOut;

void main(){
	float currentPos = UV.x*2.0-1.0;
	float actualAlpha = alphaValue; // 0.5;
	float InDepth = texture(DepthIn, UV).x;
	float OutDepth = texture(DepthOut, UV).x;
	
	
	if(currentPos > position) {
		if(InDepth == 1.0 && OutDepth == 1.0) discard;
		else if(InDepth < OutDepth) {
			color.rgb = texture(TexIn, UV).rgb;		
		} else {
			color.rgb = (1.0-actualAlpha)*texture(TexIn, UV).rgb + 
						actualAlpha*texture(TexOut, UV).rgb;
		}
	} else {
		discard;
	}
	color.w = 1.0;
	color = clamp(color, 0.0, 1.0);

	glowMap = vec4(0.0, 0.0, 0.0, 1.0);
}