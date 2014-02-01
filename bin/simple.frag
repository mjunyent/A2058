#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform float a;
uniform float cosa;

void main(){
	color = vec4(UV, a*cosa, 1.0);
}