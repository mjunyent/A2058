#version 330 core

// Ouput data
layout(location = 0) out vec4 colour;

uniform sampler2D tex;

in VertexData {
	vec2 UV;
} VertexIn;

void main()
{
	colour = texture(tex, VertexIn.UV);
	colour.a = colour.a - 0.1;
}