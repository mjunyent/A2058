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
	A3dsHandler *crossSection;

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

	int nearestRBC;

	GLFWmutex mutex;
	int *status;
	int *command;

	VesselScene(Shader *shader, GLFWmutex mutex, int *status, int *command);

	void update(double t);
		void updatePlay();
		void updateStop();

	void renderiseee(glm::mat4 &V);
	void getNearest();

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

class InfoRender : public Scene {
public:
	FBO *renderTarget;
	FBO *background;
	TBO *image;
	Shader *third;
	float boxCoords[12];
	VBO *Box;
	IBO *BoxI;

	GLint bg_Id, im_Id, topId, downId, leftId, rightId, drawId;

	VesselScene *Vs;
	glm::mat4 *V;

	InfoRender(VesselScene* Vs, glm::mat4 *V, FBO *background, FBO *target);

	void draw(double t);

};

#endif