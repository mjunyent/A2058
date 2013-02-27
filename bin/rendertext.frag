#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D color_tex;
//uniform sampler2D depth_tex;

void main(){
	color = texture( color_tex, UV );
	if(color.x < 0.1) color.w = 0.0;
	//color.w = color.x;
}