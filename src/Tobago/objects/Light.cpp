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
	tmp.Direction = normalize(Direction);
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

	lights.push_back(tmp);
}

void Light::addPointLight(vec3 Position, vec3 Attenuation, vec3 Colour) {
	int id = lights.size();
	if(id >= MAX_LIGHTS) {
		global::log.warning("NO MORE LIGHTS CAN BE ADDED!");
		return;
	}

	//create temporal lamp struct
	lamp tmp;
	//add the data
	tmp.type = POINT;
	tmp.Attenuation = Attenuation;
	tmp.Position = Position;
	tmp.Colour = Colour;

	//Get the uniforms ids
	//first we generate the correct names
	int pLoc = name.length() + 1;
	typeName[pLoc] += id;
	attName[pLoc] += id;
	posName[pLoc] += id;
	colourName[pLoc] += id;

	tmp.typeId = shader->getUniform(typeName.c_str());
	tmp.attId = shader->getUniform(attName.c_str());
	tmp.posId = shader->getUniform(posName.c_str());
	tmp.colourId = shader->getUniform(colourName.c_str());

	typeName[pLoc] = '0';
	attName[pLoc] = '0';
	posName[pLoc] = '0';
	colourName[pLoc] = '0';

	lights.push_back(tmp);
}

void Light::addSpotLight(vec3 Position,
			  			  vec3 Attenuation,
						  vec3 Direction,
						  vec3 Colour,
						  float OuterCutoff,
						  float InnerCutoff,
						  float Exponent) {
	int id = lights.size();
	if(id >= MAX_LIGHTS) {
		global::log.warning("NO MORE LIGHTS CAN BE ADDED!");
		return;
	}

	//create temporal lamp struct
	lamp tmp;
	//add the data
	tmp.type = SPOTLIGHT;
	tmp.Position = Position;
	tmp.Attenuation = Attenuation;
	tmp.Direction = Direction;
	tmp.Colour = Colour;
	tmp.OuterCutoff = OuterCutoff;
	tmp.InnerCutoff = InnerCutoff;
	tmp.Exponent = Exponent;

	//Get the uniforms ids
	//first we generate the correct names
	int pLoc = name.length() + 1;
	typeName[pLoc]   += id;
	posName[pLoc]    += id;
	attName[pLoc]    += id;
	dirName[pLoc]    += id;
	colourName[pLoc] += id;
	OCutName[pLoc]   += id;
	InCutName[pLoc]  += id;
	ExpName[pLoc]    += id;

	tmp.typeId = shader->getUniform(typeName.c_str());
	tmp.posId = shader->getUniform(posName.c_str());
	tmp.attId = shader->getUniform(attName.c_str());
	tmp.dirId = shader->getUniform(dirName.c_str());
	tmp.colourId = shader->getUniform(colourName.c_str());
	tmp.OCutId = shader->getUniform(OCutName.c_str());
	tmp.InCutId = shader->getUniform(InCutName.c_str());
	tmp.ExpId = shader->getUniform(ExpName.c_str());

	typeName[pLoc]   += '0';
	posName[pLoc]    += '0';
	attName[pLoc]    += '0';
	dirName[pLoc]    += '0';
	colourName[pLoc] += '0';
	OCutName[pLoc]   += '0';
	InCutName[pLoc]  += '0';
	ExpName[pLoc]    += '0';

	lights.push_back(tmp);
}

void Light::passLightToGPU() {
	for(lamp l : lights) {
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
			glUniform1i(l.typeId, POINT);
			glUniform3fv(l.posId, 1, &l.Position[0]);
			glUniform3fv(l.attId, 1, &l.Attenuation[0]);
			glUniform3fv(l.dirId, 1, &l.Direction[0]);
			glUniform3fv(l.colourId, 1, &l.Colour[0]);
			glUniform1f(l.OCutId, l.OuterCutoff);
			glUniform1f(l.InCutId, l.InnerCutoff);
			glUniform1f(l.ExpId, l.Exponent);
		}
	}
	glUniform1i(numLightsId, lights.size());
};