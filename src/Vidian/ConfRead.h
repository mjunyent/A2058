#pragma once
#ifndef VIDIAN_CONFREAD
#define VIDIAN_CONFREAD

#include "../Tobago/Tobago.h"
#include "../Trinidad/Trinidad.h"
#include "VesselScene.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

class ConfRead {
public:
	struct ConfMaterial {
		float ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
		float scale;
	};

	std::string filename;

	std::string MODE;
	bool frate;
	int ResX, ResY;
	int WindowMode;
	vector<float> costs;
	THREEDMODES mode3d;
	glm::vec3 position, direction, up;
	float EyeSep, CenterDist;

	ConfMaterial VesselMaterial, RBCMaterial;
	vector<Light::lamp> lights;
	int numLights;

	ConfRead(std::string name);

	void read();
	void UpdateModels(VesselScene *Vs);
	void UpdateLights(Light *light);
};

#endif