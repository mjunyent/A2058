#include "ConfRead.h"

ConfRead::ConfRead(std::string name) {
	filename = name;
	read();
}

void ConfRead::read() {
	ifstream fs(filename);

	if(fs.is_open()) {
	std::string tmp;
	float px, py, pz;

	fs >> MODE;

	fs >> ResX >> ResY;
	fs >> tmp;
	if(tmp == "FULLSCREEN") WindowMode = GLFW_FULLSCREEN;
	else WindowMode= GLFW_WINDOW;

	for(int i=0; i<6; i++) {
		float c;
		fs >> c;
		costs.push_back(c);
	}

	fs >> tmp;
	if(tmp == "ANAGLYPH") mode3d = ANAGLYPH;
	else if(tmp == "SIDEBYSIDE") mode3d = SIDEBYSIDE;
	else if(tmp == "UPNDOWN") mode3d = UPNDOWN;
	else mode3d = NONE;

	fs >> px >> py >> pz;
	position = glm::vec3(px, py, pz);

	fs >> px >> py >> pz;
	direction = glm::vec3(px, py, pz);

	fs >> px >> py >> pz;
	up = glm::vec3(px, py, pz);

	fs >> EyeSep >> CenterDist;

	fs >> VesselMaterial.ambient;
	fs >> px >> py >> pz;
	VesselMaterial.diffuse = glm::vec3(px, py, pz);
	fs >> px >> py >> pz;
	VesselMaterial.specular = glm::vec3(px, py, pz);
	fs >> VesselMaterial.shininess >> VesselMaterial.scale;

	fs >> RBCMaterial.ambient;
	fs >> px >> py >> pz;
	RBCMaterial.diffuse = glm::vec3(px, py, pz);
	fs >> px >> py >> pz;
	RBCMaterial.specular = glm::vec3(px, py, pz);
	fs >> RBCMaterial.shininess >> RBCMaterial.scale;

	fs >> numLights;

	vector<Light::lamp> tmplights(numLights);
	for(int i=0; i<numLights; i++) {
		fs >> tmp;
		if(tmp == "DIRECTIONAL") tmplights[i].type = Light::DIRECTIONAL;
		else if(tmp == "POINT") tmplights[i].type = Light::POINT;
		else tmplights[i].type = Light::SPOTLIGHT;

		fs >> px >> py >> pz;
		tmplights[i].Position = glm::vec3(px, py, pz);
		fs >> px >> py >> pz;
		tmplights[i].Attenuation = glm::vec3(px, py, pz);
		fs >> px >> py >> pz;
		tmplights[i].Direction = glm::vec3(px, py, pz);
		fs >> px >> py >> pz;
		tmplights[i].Colour = glm::vec3(px, py, pz);
		fs >> tmplights[i].OuterCutoff >> tmplights[i].InnerCutoff >> tmplights[i].Exponent;
	}

	lights = tmplights;

	fs.close();
	}
}

void ConfRead::UpdateModels(VesselScene *Vs) {
	Vs->vessel_model->ambient_factor = VesselMaterial.ambient;
	Vs->vessel_model->diffuse_color  = VesselMaterial.diffuse;
	Vs->vessel_model->specular_color = VesselMaterial.specular;
	Vs->vessel_model->shininess		 = VesselMaterial.shininess;
	Vs->vessel_model->scale			 = VesselMaterial.scale;

	Vs->cell_model->ambient_factor	 = RBCMaterial.ambient;
	Vs->cell_model->diffuse_color	 = RBCMaterial.diffuse;
	Vs->cell_model->specular_color	 = RBCMaterial.specular;
	Vs->cell_model->shininess		 = RBCMaterial.shininess;
	Vs->cell_model->scale			 = RBCMaterial.scale;
}

void ConfRead::UpdateLights(Light *light) {
	light->numLights = numLights;

	for(int i=0; i<numLights; i++) {
		light->lights[i].type		 = lights[i].type;
		light->lights[i].Position	 = lights[i].Position;
		light->lights[i].Attenuation = lights[i].Attenuation;
		light->lights[i].Direction	 = lights[i].Direction;
		light->lights[i].Colour		 = lights[i].Colour;
		light->lights[i].OuterCutoff = lights[i].OuterCutoff;
		light->lights[i].InnerCutoff = lights[i].InnerCutoff;
		light->lights[i].Exponent	 = lights[i].Exponent;
	}

}