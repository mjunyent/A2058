#version 330 core

in vec2 UV;
out vec4 Color;

uniform sampler2D tex;

void main(){
		vec4 c= texture(tex,UV);
		Color = vec4(c.g,c.b,c.r,c.a);
}