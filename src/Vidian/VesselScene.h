#pragma once
#ifndef VIDIAN_VESSELSCENE
#define VIDIAN_VESSELSCENE

#include "../Tobago/Tobago.h"
#include "../Trinidad/Trinidad.h"
#include <cstdlib>
#include <windows.h>

/**
	Prerequisites:
		-shader must have: layout0 = vec3 for Vertexs.
						   layout1 = vec3 for Normals.
		-Uniform Mat4 named Model, View and Projection to allocate the MVP matrix separately.
		-Uniform float scale to specify the scale factor.
*/
class Erythrocyte {
public:
	glm::mat4 M;	//(Resultant) Translate matrix

	glm::vec3 position;
	glm::vec3 rotateV;

	float angle;
	float angv;

	float p;
	float v;
	float a;

	float pY, pZ;

	Erythrocyte(float start_position, float range_Z, float range_Y);
	void update();
//	void recycle();

	glm::vec3 bezier(double t);
	glm::vec2 dbezier(double t);
};

class VesselScene : public Scene {
public:
	A3dsHandler *vessel;
	A3dsHandler *cell;

	Model *vessel_model;
	Model *cell_model;

	glm::mat4 M_Vessel, *V, P, invPV;
	vector<Erythrocyte> globuline;
	GLuint M_Id, V_Id, P_Id, Scale_Id;
	Shader *shader;
	Rig *rig;

	bool Beat;
	bool VelSet;
	float Global_acc;
	float Global_vel;

	GLFWmutex mutex;
	int *status;
	int *command;

	VesselScene(Shader *shader, GLFWmutex mutex, int *status, int *command);

	void update(double t);
		void updatePlay();
		void updateStop();
	void renderiseee(glm::mat4 &V);

private:
	glm::vec3 bezier(double t);
};


class VesselRender : public Scene {
public:
	Shader *shader;
	glm::mat4 invPV;
	glm::mat4 *V;
	VesselScene *Vs;

	VesselRender(VesselScene* Vs, glm::mat4 *V);

	void draw(double t);
};

#endif