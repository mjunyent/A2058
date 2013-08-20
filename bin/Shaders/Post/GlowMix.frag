#version 330 core
//http://devmaster.net/posts/3100/shader-effects-glow-and-bloom
in vec2 UV;
layout(location = 0) out vec4 color;

uniform sampler2D 	TexA;
uniform sampler2D	TexB;
uniform int			Mode;

void main(){
	vec3 glowmap = texture(TexA, UV).rgb;
	vec3 render  = texture(TexB, UV).rgb;
	if(Mode == 0) {
		color.rgb = glowmap + render;
	} else if(Mode == 1) {
		color.rgb = (glowmap + render) - (glowmap*render);
	}
//	color.rgb = texture(TexB, UV).rgb;
	color.w = 1.0;
	color = clamp(color, 0.0, 1.0);
}