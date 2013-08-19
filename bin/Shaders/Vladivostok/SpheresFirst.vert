#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexTangent_modelspace;
layout(location = 4) in vec3 vertexBitangent_modelspace;

out vec4 normal;
out vec2 UV;
out mat3 TBN;

// Values that stay constant for the whole mesh.
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float scale;

void main() {
	mat4 MVP = Projection*View*Model;

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(scale*vertexPosition_modelspace,1);

	TBN = mat3(normalize(vertexTangent_modelspace),
			   normalize(vertexBitangent_modelspace),
			   normalize(vertexNormal_modelspace));

	normal = Model*vec4(vertexNormal_modelspace, 0);

	UV = vertexUV;
}

