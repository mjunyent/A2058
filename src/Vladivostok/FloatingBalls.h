#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "Parser.h"

class FloatingBalls : public Deferred {
public:
	TBO ballGradient;
	GLint ballGradientID;
	Camera *myCam;
	Rig *myRig;

	Sphere *sphere;
	SkyBox *box;
	Model *sphere_model, *box_model;
	glm::mat4 M_sphere, M_box, M_world;
	vector<vec3> M_ball;

	FloatingBalls(CSParser *csp);

	void SecondPass();
	void render(int s, double t);

	void update(double t);

private:
	CSParser *csp;
	bool renderBox;
};