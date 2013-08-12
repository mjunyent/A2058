#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"

class Spheres : public Deferred {
public:
	Sphere *sp;
	A3dsHandler *vessel;
	Model *vessel_model;
	glm::mat4 M_Vessel;

	Spheres(Camera *cam);

	void render(int s, double t);

private:

};