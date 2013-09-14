#version 150 core

layout (points) in;
layout (triangle_strip, max_vertices=4) out;

out VertexData {
	vec2 UV;
} VertexOut;

/* Built-in IN variables 
	in gl_PerVertex {
		vec4  gl_Position;
		float gl_PointSize;
		float gl_ClipDistance[];
	} gl_in[];

	in int gl_PrimitiveIDIn;
	// only for OpenGL 4.0+
	in int gl_InvocationID;

Built-in OUT variables
	out gl_PerVertex {
		vec4  gl_Position;
		float gl_PointSize;
		float gl_ClipDistance[];
	};

	out int gl_PrimitiveID;
	out int gl_Layer;
	// only for OpenGL 4.1+
	out int gl_ViewportIndex;
*/

uniform mat4 View;
uniform mat4 Projection;

uniform vec3 up;
uniform vec3 camPos;
uniform float r;

void main(void)
{
	mat4 MVP = Projection*View;

	vec3 d = gl_in[0].gl_Position.xyz - camPos;
	
	//In plane perpendicular vectors.
	vec3 left = normalize(cross(up, d));
	vec3 upp = normalize(cross(d, left));
	
	//Upp Left
	vec4 vert = vec4(gl_in[0].gl_Position.xyz + r*upp + r*left, 1.0);
	gl_Position = MVP*vert;
	VertexOut.UV = vec2(0.0, 0.0);
	EmitVertex();
	
	//Down Left
	vert = vec4(gl_in[0].gl_Position.xyz - r*upp + r*left, 1.0);
	gl_Position = MVP*vert;
	VertexOut.UV = vec2(0.0, 1.0);
	EmitVertex();
	
	//Upp Right
	vert = vec4(gl_in[0].gl_Position.xyz + r*upp - r*left, 1.0);
	gl_Position = MVP*vert;
	VertexOut.UV = vec2(1.0, 0.0);
	EmitVertex();

	//Down Right
	vert = vec4(gl_in[0].gl_Position.xyz - r*upp - r*left, 1.0);
	gl_Position = MVP*vert;
	VertexOut.UV = vec2(1.0, 1.0);
	EmitVertex();
}
