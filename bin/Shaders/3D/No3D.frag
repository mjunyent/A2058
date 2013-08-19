#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D LeftTex;

void main(){
	color = vec4(texture(LeftTex, UV).rgb, 1.0);
}