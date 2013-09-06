#version 330 core

// Ouput data
layout(location = 0) out vec4 colour;

in VertexData {
	vec2 UV;
} VertexIn;

void main()
{
	colour = vec4(VertexIn.UV.x, VertexIn.UV.y, 1.0, 1.0);
}