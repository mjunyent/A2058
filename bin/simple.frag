#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

void main(){
	color = vec4(UV, 1.0, 1.0);
}