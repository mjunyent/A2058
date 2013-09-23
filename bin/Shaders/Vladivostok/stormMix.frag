#version 330 core
//http://devmaster.net/posts/3100/shader-effects-glow-and-bloom
in vec2 UV;
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 glowMap;
//layout (depth_unchanged) out float gl_FragDepth;

uniform float		position;

uniform sampler2D 	TexLeft;
uniform sampler2D	TexRight;

uniform sampler2D	DepthLeft;
uniform sampler2D	DepthRight;

uniform bool		showLeft;
uniform bool		showRight;

void main(){
	float currentPos = UV.x*2.0-1.0;
	
	if(showLeft && currentPos < position) {
		if(texture(DepthLeft, UV).x == 1.0) discard;
		else {
			color.rgb = texture(TexLeft, UV).rgb;
			color.w = 1.0;
			color = clamp(color, 0.0, 1.0);
		}	
	} else if(showRight && currentPos > position) {
		if(texture(DepthRight, UV).x == 1.0) discard;
		else {
			color.rgb = texture(TexRight, UV).rgb;
			color.w = 1.0;
			color = clamp(color, 0.0, 1.0);
		}
	}


	glowMap = vec4(0.0, 0.0, 0.0, 1.0);
}