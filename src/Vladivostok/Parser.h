#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <string>
#include <sstream>

class CSParser {
public:
	struct Data {
		int width, height;
		std::string windowTitle;
		int oglMajor, oglMinor;

		float FOV, zNear, zFar;
		glm::vec3 CamPos, CamDir, CamUp;

		float FocalLength, FocusDistance, FStop;

		float EyeSep;

		int nlights;
		vector<Light::lamp> lights;
	} data;

	CSParser(std::string filename);

	void parse();

	float getf(const std::string& s);
	glm::vec3 getvec3(const std::string& s);

	glm::vec3 toVec3(const std::string& s);

	void readLights(const char* prefix); // "Model.Lights"
	void passToLight(Light *l);
	void passToCam(Camera *c);
	void passToRig(Rig *r);

	boost::property_tree::ptree pr;
	std::string filename;
};