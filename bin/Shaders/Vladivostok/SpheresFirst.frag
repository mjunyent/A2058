#version 330 core

// Ouput data
layout(location = 0) out vec4 normal_c;
layout(location = 1) out vec4 diffuse;
layout(location = 2) out vec4 specular;
layout(location = 3) out vec4 emitBuff;

in vec4 normal;
in vec2 UV;
in mat3 TBN;

uniform float shininess;
uniform float AmbientFactor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;

uniform bool isNormalTex;
uniform sampler2D NormalTex;
uniform float emissive;

void main()
{
	if(isNormalTex) {
	    vec3 temp = normalize(TBN*(texture( NormalTex, vec2(UV.x, -UV.y) ).xyz*2.0-1.0));
		normal_c = vec4( (temp+vec3(1.0, 1.0, 1.0))/2.0, AmbientFactor );
	} else {
		normal_c = vec4((normalize(normal.xyz)+vec3(1.0,1.0,1.0))/2.0, AmbientFactor);
	}
	diffuse  = vec4(DiffuseColor.rgb, 0.0);
	specular = vec4(SpecularColor.rgb, shininess);
	emitBuff.x = emissive;
}