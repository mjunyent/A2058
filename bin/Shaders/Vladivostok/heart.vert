#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 textCoordinates;

// Values that stay constant for the whole mesh.
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 UV;

void main() {
	mat4 MVP = Projection*View*Model;

	UV = (textCoordinates.xy + vec2(1.0, 1.0))/2.0;

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
}

