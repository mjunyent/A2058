#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <string>

class FloatingBalls : public Deferred {
public:
	Camera *myCam;
	Rig *myRig;

	Sphere *sphere;
	SkyBox *box;
	Model *sphere_model, *box_model;
	glm::mat4 M_sphere, M_box, M_world;
	
	boost::property_tree::ptree properties;

	FloatingBalls();

	void render(int s, double t);

	void update(double t);

private:

};