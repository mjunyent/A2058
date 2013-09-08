#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

// Output data ; will be interpolated for each fragment.
out VertexData {
	vec2 UV;
} VertexOut;
	
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main(){
	VertexOut.UV = (vertexPosition_modelspace.xy+vec2(1,1))/2.0;
	
	//mat4 MVP = Projection*View*Model;

	gl_Position = Model*vec4(vertexPosition_modelspace,1);

	//normal = Model*vec4(vertexNormal_modelspace, 0);
}

