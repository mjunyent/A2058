#version 330 core
//http://devmaster.net/posts/3100/shader-effects-glow-and-bloom
in vec2 UV;
layout(location = 0) out vec4 color;
//layout (depth_unchanged) out float gl_FragDepth;

uniform sampler2D 	Tex;
uniform sampler2D	Depth;
uniform float		position;

void main(){
	color.rgb = texture(Tex, UV).rgb;
	if(UV.x*2.0-1.0 < position) discard;
	if(texture(Depth, UV).x == 1.0) discard;
	color.w = 1.0;
	color = clamp(color, 0.0, 1.0);
//	if(1==2) gl_FragDepth = 0.0f;
}