#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D Tex;
uniform float till;
void main(){
	if(till > UV.x)	color = vec4(texture(Tex, vec2(UV.x, UV.y)).rgb , 1.0);
	else color = vec4(0.0,0.0,0.0,1.0);
}