#include "Light.h"

Light::Light(Shader *shader, std::string name) {
	this->name = name;
	this->shader = shader;

	numLightsId = shader->getUniform("nlights");

	typeName = name + "[0].type";
	posName = name + "[0].Position";
	attName = name + "[0].Attenuation";
	dirName = name + "[0].Direction";
	colourName = name + "[0].Colour";
	OCutName = name + "[0].OuterCutoff";
	InCutName = name + "[0].InnerCutoff";
	ExpName = name + "[0].Exponent";
/*
	strcpy(typeName, name);
    strcat(typeName, "[0].type");
	
	strcpy(posName, name);
    strcat(posName, "[0].Position");

	strcpy(attName, name);
    strcat(attName, "[0].Attenuation");

	strcpy(dirName, name);
    strcat(dirName, "[0].Direction");

	strcpy(colourName, name);
    strcat(colourName, "[0].Colour");

	strcpy(OCutName, name);
    strcat(OCutName, "[0].OuterCutoff");

	strcpy(InCutName, name);
    strcat(InCutName, "[0].InnerCutoff");

	strcpy(ExpName, name);
    strcat(ExpName, "[0].Exponent");
	*/
}

void Light::addDirectionalLight(vec3 Attenuation, vec3 Direction, vec3 Colour) {
	int id = lights.size();
	if(id >= MAX_LIGHTS) {
		global::log.warning("NO MORE LIGHTS CAN BE ADDED!");
		return;
	}

	//create temporal lamp struct
	lamp tmp;
	//add the data
	tmp.type = DIRECTIONAL;
	tmp.Attenuation = Attenuation;
	tmp.Direction = Direction;
	tmp.Colour = Colour;

	//Get the uniforms ids
	//first we generate the correct names
	int pLoc = name.length() + 1;
	typeName[pLoc] += id;
	attName[pLoc] += id;
	dirName[pLoc] += id;
	colourName[pLoc] += id;

	tmp.typeId = shader->getUniform(typeName.c_str());
	tmp.attId = shader->getUniform(attName.c_str());
	tmp.dirId = shader->getUniform(dirName.c_str());
	tmp.colourId = shader->getUniform(colourName.c_str());

	typeName[pLoc] = '0';
	attName[pLoc] = '0';
	dirName[pLoc] = '0';
	colourName[pLoc] = '0';
}

void Light::passLightToGPU() {
	for(lamp l : lights) {
		if(l.type == DIRECTIONAL) {
			glUniform1i(l.typeId, DIRECTIONAL);
			glUniform3fv(l.attId, 1, &l.Attenuation[0]);
			glUniform3fv(l.dirId, 1, &l.Direction[0]);
			glUniform3fv(l.colourId, 1, &l.Colour[0]);
		}
	}
	glUniform1i(numLightsId, lights.size());
};