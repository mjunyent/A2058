#pragma once

#include "Tobago/Tobago.h"
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <string>
#include <sstream>

class CSParser {
public:
	struct Data {
		float FOV, zNear, zFar;
		glm::vec3 CamPos, CamDir, CamUp;

		float FocalLength, FocusDistance, FStop;

		float EyeSep;
	} data;

	CSParser(std::string filename);

	void parse();

	glm::vec3 toVec3(const std::string& s);

private:
	boost::property_tree::ptree pr;
	std::string filename;
};