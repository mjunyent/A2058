#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D LeftTex;
uniform sampler2D RightTex;

void main(){
	if(UV.y < 0.5) color = vec4(texture(LeftTex, vec2(UV.x, UV.y*2)).rgb , 1.0);
	if(UV.y > 0.5) color = vec4(texture(RightTex, vec2(UV.x, UV.y*2)).rgb , 1.0);
}