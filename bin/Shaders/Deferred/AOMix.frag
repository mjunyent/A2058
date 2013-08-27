#version 330 core
//#pragma optimize(off)

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D AOTex;
uniform sampler2D Tex;

void main(){
	vec3 c = texture(Tex, UV).rgb;
	vec3 ao = texture(AOTex, UV).rgb;
	
	color.rgb = clamp(c-ao, 0.0, 1.0);
	color.w = 1.0;
}












