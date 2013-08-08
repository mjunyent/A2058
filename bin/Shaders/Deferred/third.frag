#version 330 core
//#pragma optimize(off)

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D Normal;
uniform sampler2D Depth;
uniform sampler2D Final;

uniform float width;
uniform float height;

void main(){
	vec2 screen = vec2(1.0/width, 1.0/height);

	vec2 offset[9] = vec2[](
		vec2(-1,-1),
		vec2(-1, 1),
		vec2( 1,-1),
		vec2( 1, 1),
		vec2( 0, 1),
		vec2( 0,-1),
		vec2( 1, 0),
		vec2(-1, 0),
		vec2( 0, 0)
		);


	vec3 n = texture(Normal, UV).xyz;
	float d = texture(Depth, UV).x;

	float factor = 0.0;

	for(int i=0; i<8; i++) {
		vec3 t = texture(Normal, UV+offset[i]*screen).xyz;
		factor += dot(t-n, t-n);
		float tt = texture(Depth, UV+offset[i]*screen).x-d;
		factor += tt*tt;
	}

	factor = min(1.0, factor)*1.0;


	color.rgb = 2.0*texture(Final, UV).rgb;

	for(int i=0; i<8; i++) {
		color.rgb += texture(Final, UV+offset[i]*screen).rgb*factor;
	}

	color = color / (2.0+8.0*factor);
	//color = vec4(factor, factor, factor, 1.0);
}