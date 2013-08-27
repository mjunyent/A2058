#version 330 core
//#pragma optimize(off)

in vec2 UV;

layout(location = 0) out vec4 color;

uniform sampler2D Normal;
uniform sampler2D Depth;
uniform sampler2D NormalMap;

uniform vec2 TexelSize;

uniform float OccluderBias;
uniform float SamplingRadius;
uniform vec2 Attenuation;

uniform mat4 invPV;

//http://webglfactory.blogspot.com.es/2011/05/how-to-convert-world-to-screen.html
vec4 get3dPoint(in vec2 p) {
	float depth = texture(Depth, p).x;
	vec4 point = vec4( 2*p.x-1, 2*p.y-1, 2*depth-1, 1.0);
	point = invPV*point;
	return point/point.w;
}

//Sample AO at the UV coordinate.
//srcPos: Source pixel position.
//srcNormal: Source pixel normal.
//UV: coordinate to sample.
//Returns: AO Amount
//http://devmaster.net/posts/3095/shader-effects-screen-space-ambient-occlusion
float SamplePixels(vec3 srcPosition, vec3 srcNormal, vec2 UV) {
	vec3 dstPosition = get3dPoint(UV).xyz;

	vec3 positionVec = dstPosition - srcPosition;
	float intensity = max(dot(normalize(positionVec), srcNormal) - OccluderBias, 0.0);
	float dist = length(positionVec);
	float attenuation = 1.0/(Attenuation.x + (Attenuation.y*dist));
	
	return intensity*attenuation;
}

void main(){
	vec3 srcPosition = get3dPoint(UV).xyz;
	vec3 srcNormal = texture(Normal, UV).xyz*2.0 - vec3(1.0, 1.0, 1.0);
	vec2 randVec = normalize(texture2D(NormalMap, UV).xy * 2.0 - 1.0);
	float srcDepth = texture(Depth, UV).x;
	
	float kernelRadius = SamplingRadius * (1.0 - srcDepth);
	
	vec2 kernel[4];
	kernel[0] = vec2( 0.0,  1.0);
	kernel[1] = vec2( 1.0,  0.0);
	kernel[2] = vec2( 0.0, -1.0);
	kernel[3] = vec2(-1.0,  0.0);

	const float Sin45 = 0.707107;
	
	float occlusion = 0.0;
	for(int i=0; i<4; ++i) {
		vec2 k1 = reflect(kernel[i], randVec);
		vec2 k2 = vec2(k1.x*Sin45 - k1.y*Sin45, k1.x*Sin45 + k1.y*Sin45);

		k1 *= TexelSize;
		k2 *= TexelSize;
		
		occlusion += SamplePixels(srcPosition, srcNormal, UV + k1*kernelRadius);
		occlusion += SamplePixels(srcPosition, srcNormal, UV + k2*kernelRadius*0.75);
		occlusion += SamplePixels(srcPosition, srcNormal, UV + k1*kernelRadius*0.5);
		occlusion += SamplePixels(srcPosition, srcNormal, UV + k2*kernelRadius*0.25);
	}
	
	occlusion /= 16.0;
	occlusion = clamp(occlusion, 0.0, 1.0);

	color.rgb = vec3(occlusion);
	color.w = 1.0;
}












