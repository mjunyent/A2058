#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;

uniform mat4 invPersp;

//http://www.opengl.org/discussion_boards/showthread.php/173019-View-Space-Light-Position-Moving?p=1212628&viewfull=1#post1212628
//vec3 PositionFromDepth_DarkPhoton(in float depth)

//http://stackoverflow.com/questions/11277501/how-to-recover-view-space-position-given-view-space-depth-value-and-ndc-xy
vec3 getFragmentPosition()
{
     vec4 sPos = vec4(UV.x,
					  UV.y,
					  texture(tex4, UV).x, 1.0);
     sPos.z = 2.0 * (sPos.z-0.5);
     sPos = invPersp * sPos;

     return sPos.xyz / sPos.w;
}

void main(){
	color.w = 1.0;

	if(UV.x < 0.5 && UV.y > 0.5) color.rgb = texture( tex1, 2*UV ).rgb;						//superior izq.
	if(UV.x > 0.5 && UV.y > 0.5) color.rgb = texture( tex2, 2*UV ).rgb;						//superior der.
	if(UV.x < 0.5 && UV.y < 0.5) color.rgb = abs(texture( tex3, 2*UV ).rgb);						//inferior iqz.
	if(UV.x > 0.5 && UV.y < 0.5) {
		color.rgb = texture( tex4, 2*UV ).rgb;						//inferior der.
	//	color.rgb = getFragmentPosition();
	}

	//color.rgb = getFragmentPosition();
}