#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;

void main(){
	color.w = 1.0;
//	if(color.x < 0.1) color.w = 0.0;
	//color.w = color.x;
	if(UV.x < 0.5 && UV.y < 0.5) color = texture( tex1, 2*UV );
	if(UV.x > 0.5 && UV.y < 0.5) color = texture( tex2, 2*UV );
	if(UV.x > 0.5 && UV.y > 0.5) color = texture( tex3, 2*UV );
	if(UV.x < 0.5 && UV.y > 0.5) color = vec4(1, 0, 0, 1); //texture( tex4, 2*UV );

//	color = vec4(1.0, 0.0, 0.0, 1.0);
}