#version 330 core
//#pragma optimize(off)

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D Texture;
uniform sampler2D Depth;

uniform float width;
uniform float height;

uniform float FocalLength;
uniform float FocalDistance;
uniform float N;

void main(){
	vec2 screen = vec2(1.0/width, 1.0/height);

	color.rgb = 2.0*texture(Texture, UV).rgb;
	color.w = 1.0;
}