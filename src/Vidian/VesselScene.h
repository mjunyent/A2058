#pragma once
#ifndef VIDIAN_VESSELSCENE
#define VIDIAN_VESSELSCENE

#include "../Tobago/Tobago.h"
#include "../Trinidad/Trinidad.h"
#include <cstdlib>

/**
	Prerequisites:
		-shader must have: layout0 = vec3 for Vertexs.
						   layout1 = vec3 for Normals.
		-Uniform Mat4 named Model, View and Projection to allocate the MVP matrix separately.
		-Uniform float scale to specify the scale factor.
*/

class VesselScene : public Scene {
public:
	A3dsHandler *vessel;
	A3dsHandler *cell;
	glm::mat4 M_Vessel, *V, P;
	vector<glm::mat4> M_Cells;
	GLuint M_Id, V_Id, P_Id, Scale_Id;
	Shader *shader;
	Rig *rig;

	VesselScene(Shader *shader, glm::mat4 *V);
	VesselScene(Shader *shader, Rig *rig);
	void draw(double t);
	void renderiseee(glm::mat4 &V);
};

#endif