#version 330 core

// Ouput data
layout(location = 0) out vec4 normal_c;
layout(location = 1) out vec4 diffuse;
layout(location = 2) out vec4 specular;

in vec4 normal;
in vec4 position;
in vec4 deletethis;

uniform float shininess;
uniform float AmbientFactor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;

void main()
{	
	normal_c = vec4(normalize(normal.xyz), AmbientFactor);
	diffuse  = vec4(DiffuseColor.rgb, 0.0);
	specular = vec4(SpecularColor.rgb, shininess);
}