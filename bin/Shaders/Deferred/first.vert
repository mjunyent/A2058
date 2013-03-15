#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float scale;

out vec4 normal;

void main() {
	mat4 MVP = Projection*View*Model;
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(scale*vertexPosition_modelspace,1);
	normal = MVP*vec4(vertexNormal_modelspace, 0);
	normal = vec4(vertexNormal_modelspace, 0);
}

