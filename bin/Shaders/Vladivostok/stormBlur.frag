#version 330 core

in vec2 UV;
layout(location = 0) out vec4 color;

uniform sampler2D 	Texture;
uniform vec2 	  	TexelSize;
uniform int			Orientation;
uniform int			BlurAmount;
uniform float		BlurStrength;

// returns gaussian value on the x-axis with specified standard deviation.
float Gaussian (float x, float deviation) {
	return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));
}

void main(){
	float deviation = BlurAmount*0.35;
 //	float deviation = BlurAmount*0.39894;
	deviation *= deviation;
	float strength = 1.0 - BlurStrength;
	
	color = texture(Texture, UV)*Gaussian(0.0, deviation);
	
	vec2 TexelGo = vec2(0.0, 0.0);
	if(Orientation == 0) {
		TexelGo.x = TexelSize.x;
	} else {
		TexelGo.y = TexelSize.y;
	}
	
	
	for(int i=1; i<BlurAmount; i++) {
		float g = Gaussian(i*strength, deviation);
		color += texture(Texture, UV + TexelGo*i)*g;
		color += texture(Texture, UV - TexelGo*i)*g;
	}

	color = clamp(color, 0.0, 1.0);
}