#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D LeftTex;
uniform sampler2D RightTex;

void main(){
	vec4 leftFrag = texture(LeftTex, UV);
	leftFrag = vec4(leftFrag.r, 1.0, leftFrag.b, 1.0);

	vec4 rightFrag = texture(RightTex, UV);
	rightFrag = vec4(1.0, rightFrag.g, 1.0, 1.0);

	color = vec4(leftFrag.rgb * rightFrag.rgb, 1.0);
//	if(UV.x < 0.5) color = vec4(texture(LeftTex, vec2(UV.x*2, UV.y)).rgb , 1.0);
//	if(UV.x > 0.5) color = vec4(texture(RightTex, vec2(UV.x*2, UV.y)).rgb , 1.0);
}