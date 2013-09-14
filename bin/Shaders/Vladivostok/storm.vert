#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 Model;

void main(){
	gl_Position = Model*vec4(vertexPosition_modelspace,1);
}

