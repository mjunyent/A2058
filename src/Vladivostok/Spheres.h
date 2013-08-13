#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"

class Spheres : public Deferred {
public:
	A3dsHandler *vessel;
	Sphere *sp;
	Floor  *fl;
	Model *sphere_model, *floor_model;
	glm::mat4 M_sphere, M_floor;

	Spheres(Camera *cam);

	void render(int s, double t);

	void update(double t);

private:

};