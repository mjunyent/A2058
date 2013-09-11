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
	vec2 centerUV = (VertexIn.UV - vec2(0.5, 0.5))*2.0; //Coordinates centered on the center.

	colour = texture(tex, VertexIn.UV); //Get the color from the texture
	if(colour.a < 0.2) discard;
/*
	float r = mix(0.5, 1.8, clamp(pow(depth, 2.0), 0.0, 1.0)); //Calculate Cut Radius, according to depth.
	float rr = length(centerUV); //Calculate current pixel's radius.
	if(rr > r) discard; //discard if bigger.
	if(rr > 0.3) colour.a = mix(1.0, 0.0, (rr-0.3)/(r-0.3)); //set the alpha gradient if we are further than 0.3
	colour.a = pow(colour.a, 2.0); //power to the alpha (smoother gradiet).
	if(colour.a < 0.2) discard; //discard almost absolutely transparent pixels
	colour = clamp(colour, 0.0, 1.0); //clamp results*/
}

//dilluns 19:45-20:30 dilluns 16