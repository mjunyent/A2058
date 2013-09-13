#version 330 core

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D Texture;
uniform sampler2D Depth;

uniform float width;
uniform float height;

uniform float BlurCoeff;
uniform float FocalDistance;
uniform int Orientation;
uniform float Far;
uniform float Near;

void main(){
	float MAX_BLUR_RADIUS = 100.0;

	float depth = 2.0*texture(Depth, UV).x - 1.0;
	//Linearize Depth.
	float Dd = 2.0*Near*Far / (Far+Near - depth*(Far-Near)); //http://stackoverflow.com/questions/6652253/getting-the-true-z-value-from-the-depth-buffer
	float xd = abs(Dd - FocalDistance);
	float xdd = (Dd < FocalDistance) ? (FocalDistance - xd) : (FocalDistance + xd);
	float blurAmount = min(floor( 0.5 * BlurCoeff * (xd / xdd)), MAX_BLUR_RADIUS);
	//if(depth == 1.0) blurAmount = 0.0;
	
	//Do the Blur
	float count = 1.0;
	color.rgb = texture(Texture, UV).rgb;
	vec2 Offset;
	if( Orientation == 0 )
		Offset = vec2(0.0, 1.0/height);
	else
		Offset = vec2(1.0/width, 0.0);
	
	for(float i = 1.0; i<blurAmount; ++i) {
		if(UV.x+Offset.x*i > 1.0 || UV.y+Offset.y*i > 1.0) break;
		color.rgb += texture(Texture, UV + Offset*i).rgb;
		count++;
	}

	for(float i = 1.0; i<blurAmount; ++i) {
		if(UV.x-Offset.x*i < 0.0 || UV.y-Offset.y*i < 0.0) break;
		color.rgb += texture(Texture, UV - Offset*i).rgb;
		count++;
	}
	
	
	color.rgb = color.rgb/count;
	//color.rgb = vec3(blurAmount/50.0);
	color.w = 1.0;
}