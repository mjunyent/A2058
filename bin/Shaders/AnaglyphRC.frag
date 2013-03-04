#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D LeftTex;
uniform sampler2D RightTex;

void main(){
	vec4 leftFrag = texture(LeftTex, UV);
	leftFrag = vec4(1.0, leftFrag.g, leftFrag.b, 1.0);

	vec4 rightFrag = texture(RightTex, UV);
	rightFrag = vec4(rightFrag.r, 1.0, 1.0, 1.0);

	color = vec4(leftFrag.rgb * rightFrag.rgb, 1.0);
//	color = vec4(texture( LeftTex, UV ).r, texture( RightTex, UV).r, 0.0, 1.0);
}