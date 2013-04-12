#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;

uniform mat4 invPersp;

//http://www.opengl.org/discussion_boards/showthread.php/173019-View-Space-Light-Position-Moving?p=1212628&viewfull=1#post1212628
//http://stackoverflow.com/questions/11277501/how-to-recover-view-space-position-given-view-space-depth-value-and-ndc-xy

//http://webglfactory.blogspot.com.es/2011/05/how-to-convert-world-to-screen.html
vec4 get3dPoint(in vec2 p) {
	float depth = texture( tex4, p ).x;
//	if(depth == 1.0) return vec4(0.0, 0.0, 0.0, 1.0);
	vec4 point = vec4( 2*p.x-1, 2*p.y-1, 2*depth-1, 1.0);
	
	point = invPersp*point;
	return point/point.w;
}

void main(){

	if(UV.x < 0.5 && UV.y > 0.5) color = texture( tex1, 2*UV );						//superior izq.
	if(UV.x > 0.5 && UV.y > 0.5) color = texture( tex2, 2*UV );						//superior der.
	if(UV.x < 0.5 && UV.y < 0.5) color = texture( tex3, 2*UV );						//inferior iqz.
	if(UV.x > 0.5 && UV.y < 0.5) {
		color.rgb = get3dPoint(vec2(2*UV.x-1, 2*UV.y)).rgb;
//		color.rgb = texture( tex4, 2*UV ).rgb;						//inferior der.
	}

	color.w = 1.0;

}