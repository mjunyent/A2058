#version 330 core

in vec2 UV;
layout(location = 0) out vec4 color;

uniform sampler2D 	Texture;
uniform sampler2D	Map;

void main(){
	float mapValue = texture(Map, UV).x;
	color.rgb = texture(Texture, UV).rgb;
	if(color.r != 1.0 && color.g != 1.0) color.rgb = vec3(0.0);
	color.w = 1.0;
}