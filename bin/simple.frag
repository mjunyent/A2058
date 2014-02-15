#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform float azul;

void main(){
	color = vec4(UV, azul, 1.0);
}