#include "Light.h"

Light::Light(Shader *shader, std::string name) {
	this->name = name;
	this->shader = shader;

	lights = vector<lamp>(MAX_LIGHTS);
	numLights = 0;
	numLightsId = shader->getUniform("nlights");

	std::string typeName = name + "[0].type";
	std::string posName = name + "[0].Position";
	std::string attName = name + "[0].Attenuation";
	std::string dirName = name + "[0].Direction";
	std::string colourName = name + "[0].Colour";
	std::string OCutName = name + "[0].OuterCutoff";
	std::string InCutName = name + "[0].InnerCutoff";
	std::string ExpName = name + "[0].Exponent";

	int pLoc = name.length() + 1;

	for(int i=0; i<MAX_LIGHTS; i++) {
		lights[i].typeId = shader->getUniform(typeName.c_str());
		lights[i].posId = shader->getUniform(posName.c_str());
		lights[i].attId = shader->getUniform(attName.c_str());
		lights[i].dirId = shader->getUniform(dirName.c_str());
		lights[i].colourId = shader->getUniform(colourName.c_str());
		lights[i].OCutId = shader->getUniform(OCutName.c_str());
		lights[i].InCutId = shader->getUniform(InCutName.c_str());
		lights[i].ExpId = shader->getUniform(ExpName.c_str());

		typeName[pLoc]++;
		posName[pLoc]++;
		attName[pLoc]++;
		dirName[pLoc]++;
		colourName[pLoc]++;
		OCutName[pLoc]++;
		InCutName[pLoc]++;
		ExpName[pLoc]++;
	}
}

void Light::addDirectionalLight(vec3 Attenuation, vec3 Direction, vec3 Colour) {
	if(numLights >= MAX_LIGHTS) {
		global::log.warning("NO MORE LIGHTS CAN BE ADDED!");
		return;
	}

	//add the data
	lights[numLights].type = DIRECTIONAL;
	lights[numLights].Attenuation = Attenuation;
	lights[numLights].Direction = normalize(Direction);
	lights[numLights].Colour = Colour;

	numLights++;
}

void Light::addPointLight(vec3 Position, vec3 Attenuation, vec3 Colour) {
	if(numLights >= MAX_LIGHTS) {
		global::log.warning("NO MORE LIGHTS CAN BE ADDED!");
		return;
	}

	//add the data
	lights[numLights].type = POINT;
	lights[numLights].Attenuation = Attenuation;
	lights[numLights].Position = Position;
	lights[numLights].Colour = Colour;

	numLights++;
}

void Light::addSpotLight(vec3 Position,
			  			  vec3 Attenuation,
						  vec3 Direction,
						  vec3 Colour,
						  float OuterCutoff,
						  float InnerCutoff,
						  float Exponent) {
	if(numLights >= MAX_LIGHTS) {
		global::log.warning("NO MORE LIGHTS CAN BE ADDED!");
		return;
	}

	//add the data
	lights[numLights].type = SPOTLIGHT;
	lights[numLights].Position = Position;
	lights[numLights].Attenuation = Attenuation;
	lights[numLights].Direction = Direction;
	lights[numLights].Colour = Colour;
	lights[numLights].OuterCutoff = OuterCutoff;
	lights[numLights].InnerCutoff = InnerCutoff;
	lights[numLights].Exponent = Exponent;

	numLights++;
}

void Light::passLightToGPU() {
	for(int i=0; i<numLights; i++) {
		lamp l = lights[i];

		if(l.type == DIRECTIONAL) {
			glUniform1i(l.typeId, DIRECTIONAL);
			glUniform3fv(l.attId, 1, &l.Attenuation[0]);
			glUniform3fv(l.dirId, 1, &l.Direction[0]);
			glUniform3fv(l.colourId, 1, &l.Colour[0]);
		} else if(l.type == POINT) {
			glUniform1i(l.typeId, POINT);
			glUniform3fv(l.attId, 1, &l.Attenuation[0]);
			glUniform3fv(l.posId, 1, &l.Position[0]);
			glUniform3fv(l.colourId, 1, &l.Colour[0]);
		} else if(l.type == SPOTLIGHT) {
			glUniform1i(l.typeId, SPOTLIGHT);
			glUniform3fv(l.posId, 1, &l.Position[0]);
			glUniform3fv(l.attId, 1, &l.Attenuation[0]);
			glUniform3fv(l.dirId, 1, &l.Direction[0]);
			glUniform3fv(l.colourId, 1, &l.Colour[0]);
			glUniform1f(l.OCutId, l.OuterCutoff);
			glUniform1f(l.InCutId, l.InnerCutoff);
			glUniform1f(l.ExpId, l.Exponent);
		}
	}
	glUniform1i(numLightsId, numLights);
};