#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;

void main(){
	color.w = 1.0;

	if(UV.x < 0.5 && UV.y > 0.5) color.rgb = texture( tex1, 2*UV ).rgb;						//superior izq.
	if(UV.x > 0.5 && UV.y > 0.5) color.rgb = texture( tex2, 2*UV ).rgb;						//superior der.
	if(UV.x < 0.5 && UV.y < 0.5) color.rgb = texture( tex3, 2*UV ).rgb;						//inferior iqz.
	if(UV.x > 0.5 && UV.y < 0.5) color.rgb = texture( tex4, 2*UV ).rgb;						//inferior der.

}