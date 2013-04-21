#pragma once

#include "../Tobago.h"
#include <vector>
#include <cstring>

using namespace std;
using namespace glm;

class Light {
	public :
		/*Three kind of lights
		* Directional Light
		*    Position is infinitely far. (we will not use it).
		*	 We use Direction as a unitary direction vector.
		* Point Light
		*    Direction is everywhere. Vector from Position to point.
		*    (we will not use it).
		* SpotLight
		*    It has a position and a direction.
		*    InnerCutoff, OuterCutoff and Exponent only apply to this.
		*/
		enum LightTypes
		{
			DIRECTIONAL,
			POINT,
			SPOTLIGHT
		};

		struct lamp {
			LightTypes type;
			vec3 Position;
			vec3 Attenuation;
			vec3 Direction;
			vec3 Colour;
			float OuterCutoff;
			float InnerCutoff;
			float Exponent;
			//intensity?

			GLint typeId,
				  posId,
				  attId,
				  dirId,
				  colourId,
				  OCutId,
				  InCutId,
				  ExpId;
		};

		std::string typeName,
					posName,
					attName,
					dirName,
					colourName,
					OCutName,
					InCutName,
					ExpName;

		Shader *shader;
		std::string name;

		static const int MAX_LIGHTS = 8;
		vector<lamp> lights;

		GLint numLightsId;

		Light(Shader *shader,				//Shader that will render the object.
			  std::string name					//Name of the light var.
			 );

		void addDirectionalLight(vec3 Attenuation, vec3 Direction, vec3 Colour);
		void addPointLight(vec3 Position, vec3 Attenuation, vec3 Colour);
	/*	void addSpotLight(vec3 Position,
			  			  vec3 Attenuation,
						  vec3 Direction,
						  vec3 Colour,
						  float OuterCutoff,
						  float InnerCutoff,
						  float Exponent);
						  */
		void passLightToGPU();

		//Implement it if you really need it.
		//void recalculateShaderIds();
		//void removeLight(int id);
};