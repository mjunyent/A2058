#version 330 core
//#pragma optimize(off)

in vec2 UV;

layout(location = 0) out vec4 color;

struct LightSource
{
	int type;
    vec3 Position;
    vec3 Attenuation;
    vec3 Direction;
    vec3 Colour;
    float OuterCutoff;
    float InnerCutoff;
    float Exponent;
};

uniform LightSource lights[8];
int nlights;

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
	float ambientFactor = texture(Normal, UV).w;
	vec3 diffuseColor = texture( Diffuse, UV ).rgb;
	vec3 specularColor = texture( Specular, UV ).rgb;
	vec3 normal = texture( Normal, UV ).xyz; //check normalization
	vec3 position = get3dPoint(UV).xyz;

	color.rgb = diffuseColor*ambientFactor;

//	for(int i=0; i<nlights; i++) {


//	}

//	color.rgb = diffuseColor*0.2 + 0.5*diffuseColor*normal.x + 0.0*diffuseColor*normal.y;

	color.w = 1.0;
}