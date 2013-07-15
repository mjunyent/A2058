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
uniform int nlights;
uniform vec3 camera_position;

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
	vec3 diffuseColor = texture( Diffuse, UV ).rgb;
	vec3 specularColor = texture( Specular, UV ).rgb;
	vec3 normal = texture( Normal, UV ).xyz; //check normalization
	float ambientFactor = texture(Normal, UV).w;
	vec3 position = get3dPoint(UV).xyz;
	float shininess = 100.0*texture(Specular, UV).a;

	color.rgb = diffuseColor*ambientFactor;

	for(int i=0; i<nlights; i++) {
		vec3 lightDir = vec3(0.0, 0.0, 0.0);
		float d = 0.0;
		float spotlight = 0.0;

		if(lights[i].type == 0) { //Directional
			lightDir = lights[i].Direction;
			d = 0;
			spotlight = 1.0;
		} else if(lights[i].type == 1) { //Point
			lightDir = normalize(lights[i].Position - position);
			d = distance(position, lights[i].Position);
			spotlight = 1.0;
		} else if(lights[i].type == 2) { //Spotlight
			lightDir = normalize(lights[i].Position - position);
			d = distance(position, lights[i].Position);

            spotlight = max(-dot(lightDir, lights[i].Direction), 0.0);
            float spotlightFade = clamp((lights[i].OuterCutoff - spotlight) / (lights[i].OuterCutoff - lights[i].InnerCutoff), 0.0, 1.0);
		//	spotlightFade = 1.0-spotlightFade;
            spotlight = pow(spotlight * spotlightFade, lights[i].Exponent);
		}


		float l  = dot(normal, lightDir);
		if(l >= 0.0) {
            vec3 r = normalize(reflect(lightDir, normal));
            float s = pow(max(dot(r, normalize(position-camera_position)), 0.0), shininess);

            float a = 1.0 / (lights[i].Attenuation.x + (lights[i].Attenuation.y * d) + (lights[i].Attenuation.z * d * d));

			color.rgb += (l*diffuseColor + s*specularColor)*lights[i].Colour*a*spotlight;
		//	color.rgb = vec3(spotlight, spotlight, spotlight);
		}
	}

	color.w = 1.0;
	color = clamp(color, 0.0, 1.0);
}