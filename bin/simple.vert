#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main(){
	vec3 p = vertexPosition;
	
/*	if(gl_InstanceID == 0) p.y = (p.y-1.0)/2.0;
	else p.y = (p.y+1.0)/2.0;
	*/
	gl_Position =  vec4(p,1);
	UV = (p.xy+vec2(1,1))/2.0;
}

