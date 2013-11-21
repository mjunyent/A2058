#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <string>

class Spheres : public Deferred {
public:
	Camera *myCam;
	Rig *myRig;

	Sphere *sp;
	Floor  *fl;
	Model *sphere_model, *floor_model;
	glm::mat4 M_sphere, M_floor;
	boost::property_tree::ptree properties;
	GLint emissiveID;

	Spheres();

	void render(int s, double t);

	void update(double t);

private:

};