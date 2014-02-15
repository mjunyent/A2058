#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
uniform mat4 MVP;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main(){
	gl_Position =  MVP*vec4(vertexPosition,1);
	UV = (vertexPosition.xy+vec2(1,1))/2.0;
}

