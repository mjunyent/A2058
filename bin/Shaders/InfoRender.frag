#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D background;
uniform sampler2D image;

uniform float top;
uniform float down;
uniform float left;
uniform float right;
uniform int draw;

void main(){
	color = texture( background, UV );
	if(draw == 1 && UV.y <= top && UV.y >= down && UV.x >= left && UV.x <= right) {
		vec4 imgColor = texture(image, vec2((UV.x-left)/(right-left), 1-(UV.y-down)/(top-down) ));
		color.rgb += imgColor.rgb*imgColor.a;
	}

	color.w = 1;
}