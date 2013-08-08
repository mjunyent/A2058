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
	glm::mat4 V_L, V_R;
	boost::property_tree::ptree properties;
	FBO *left, *right;
	Shader *Anaglyph;

	GLint lID, rID;


	MyCells(int width, int height, glm::mat4 *P, glm::mat4 *V, glm::vec3 *cam_position);

	void draw(int s, double time);
	void update(double time);

	void render(int s, double t);

private:

};