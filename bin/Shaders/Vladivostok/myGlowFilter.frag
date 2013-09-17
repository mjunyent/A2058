#version 330 core

in vec2 UV;
layout(location = 0) out vec4 color;

uniform sampler2D 	Texture;
uniform sampler2D	Map;
uniform vec2 texelSize;

void main(){
	float mapValue = texture(Map, UV).x;

	if(mapValue >= 0.5) color.rgb = texture(Texture, UV).rgb;
	else color.rgb = vec3(0.0);
	color.w = 1.0;
}