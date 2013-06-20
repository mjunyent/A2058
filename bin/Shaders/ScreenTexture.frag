#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D tex;

void main(){
	color = texture( tex, UV );
	color.w = 1.0;
//	if(color.x < 0.1) color.w = 0.0;
	//color.w = color.x;
}