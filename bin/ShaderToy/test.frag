#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform float globalTime;
uniform float songTime;
uniform vec2  res;
uniform vec2  mouse;
uniform int   button;

void main(){
	if(button == 1) {
		color.rg = mouse.rg;
		color.b = 0;
	} else {
		color.r = 0.2*abs(sin(globalTime));
		color.b = 0.2*abs(sin(songTime));
		color.rg = color.rg+UV;
	//	color.b = res.y/1280.0;
	}
	color.w = 1;
}