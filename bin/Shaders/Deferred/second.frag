#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D Normal;
uniform sampler2D Diffuse;
uniform sampler2D Specular;
uniform sampler2D Depth;

uniform mat4 invPV;

//http://webglfactory.blogspot.com.es/2011/05/how-to-convert-world-to-screen.html
vec4 get3dPoint(in vec2 p) {
	float depth = texture(Depth, p).x;
//	if(depth == 1.0) return vec4(0.0, 0.0, 0.0, 1.0);
	vec4 point = vec4( 2*p.x-1, 2*p.y-1, 2*depth-1, 1.0);

	point = invPV*point;
	return point/point.w;
}

void main(){
	if(UV.x < 0.5 && UV.y > 0.5) color = texture( Normal, 2*UV );						//superior izq.
	if(UV.x > 0.5 && UV.y > 0.5) color = texture( Diffuse, 2*UV );						//superior der.
	if(UV.x < 0.5 && UV.y < 0.5) color = texture( Specular, 2*UV );						//inferior iqz.
	if(UV.x > 0.5 && UV.y < 0.5) color.rgb = get3dPoint(vec2(2*UV.x-1, 2*UV.y)).rgb;	//inferior der.
	color.w = 1.0;
}