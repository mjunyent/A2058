#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 MVP;
// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main(){
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	UV.xy=vertexPosition_modelspace.xy;
}

