#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

#include "uniforms.frag" //esto incluye cosas

void main(){
	color = vec4(UV, a*cosa, 1.0);
}