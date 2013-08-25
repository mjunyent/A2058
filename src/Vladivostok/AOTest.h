#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <string>

class AOTest : public Deferred {
public:
	Camera *myCam;
	Rig *myRig;

	SkyBox *box;
	A3dsHandler *cilinder3DS;
	Model *cilinder_model, *box_model;
	glm::mat4 M_cilinder, M_box;
	boost::property_tree::ptree properties;

	AOTest();

	void render(int s, double t);

	void update(double t);

private:

};