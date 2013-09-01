#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <string>

class Cilinder : public Deferred {
public:
	Camera *myCam;
	Rig *myRig;

	A3dsHandler *cilinder3DS;
	Model *cilinder_model;
	glm::mat4 M_cilinder;
	boost::property_tree::ptree properties;

	Cilinder();

	void render(int s, double t);

	void update(double t);

private:

};