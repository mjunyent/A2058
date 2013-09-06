#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "Parser.h"

class Storm : public Scene {
public:
	Shader *billboardShad;

	Camera *myCam;
	Rig *myRig;
	CSParser *csp;

	VBO *screenQuad;
	IBO *screenQuad_I;

	Storm(CSParser *csp);

	void draw(int s, double t);
	void render(int s, double t);

	void update(double t);

private:
	GLint billboard_M_Id, billboard_V_Id, billboard_P_Id, billboard_up_id, billboard_camPos_Id, billboard_r_Id;
};