#version 330 core

// Ouput data
layout(location = 0) out vec4 colour;

uniform sampler2D tex;
uniform sampler2D bgTex;
uniform float texSize;
uniform float depth;

in VertexData {
	vec2 UV;
} VertexIn;

void main()
{
	colour = texture(tex, VertexIn.UV);
	colour.a = colour.a*colour.a;
	
	if(length((VertexIn.UV - vec2(0.5, 0.5))) > 0.15) colour.a = colour.a * (depth); // * (2.0-length((VertexIn.UV - vec2(0.5, 0.5)))); //colour = vec4(1.0, 0.0, 0.0, 1.0);
//	else if(length(colour.rgb) < 0.2) colour.a = length(colour.rgb);
//	colour.a = colour.a*(1-depth) * length(VertexIn.UV - vec2(0.5, 0.5));
//	colour.a = (1 - length((VertexIn.UV - vec2(0.5, 0.5))*2.0)); //*(1-depth);
//	colour = vec4(depth, 0.0, 0.0, 1.0);
/*	vec2 smallUV = VertexIn.UV + vec2(texSize/2.0 - 0.5);
	smallUV /= texSize;
	
	if(smallUV.x >= 0.0 && smallUV.x <= 1.0 && smallUV.y >= 0.0 && smallUV.y <= 1.0) {
		colour = texture(tex, smallUV);
		colour.a = 1-texture(bgTex, VertexIn.UV).a;
		colour.rgb += texture(bgTex, VertexIn.UV).rgb*(texture(bgTex, VertexIn.UV).a);
	} else {
		colour = texture(bgTex, VertexIn.UV);
//		colour = vec4(1.0, 0.0, 0.0, 1.0);
	}

	*/
	colour = clamp(colour, 0.0, 1.0);
	
	if(colour.a < 0.1) {
	 discard;
	}
//	discard;
//	if(colour.a > 0.05) discard;
}