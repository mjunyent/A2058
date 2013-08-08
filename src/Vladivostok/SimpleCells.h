#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <string>

class MyCells : public Deferred {
public:
	A3dsHandler *vessel;
	Model *vessel_model;
	glm::mat4 M_Vessel, Virus2, Virus3;
	boost::property_tree::ptree properties;


	MyCells(int width, int height, glm::mat4 *P, glm::mat4 *V, glm::vec3 *cam_position);

	void update(double time);

	void render(int s, double t);

private:

};