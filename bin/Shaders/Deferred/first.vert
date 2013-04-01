#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

out vec4 normal;
out vec4 position;
out vec4 deletethis;

// Values that stay constant for the whole mesh.
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float scale;

void main() {

	mat4 MVP = Projection*View*Model;

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(scale*vertexPosition_modelspace,1);

	position = Model * vec4(scale*vertexPosition_modelspace,1);

	deletethis = View * Model * vec4(scale*vertexPosition_modelspace,1);

//	normal = MVP*vec4(vertexNormal_modelspace, 0);
	normal = vec4(vertexNormal_modelspace, 0);
}

