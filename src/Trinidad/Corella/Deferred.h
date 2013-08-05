#pragma once
#ifndef TRINIDAD_DEFERRED
#define TRINIDAD_DEFERRED

#include "Tobago/Tobago.h"
#include "../Director/SceneManager.h"
#include "../Director/director.h"
#include "Light.h"

class Deferred : public Scene {
public: 
	Shader *firstShad;
	Shader *secondShad;
	FBO *renderBuffer;

	Light *lights;
	glm::mat4 *P, *V;
	glm::vec3 *cam_position;

	VBO *screen_quad;
	IBO *screen_quad_I;

	GLint P_Id, V_Id;
	GLint normalID, diffuseID, specularID, depthID, invPVID, camPosID;

	Deferred(int width, int height, glm::mat4 *P, glm::mat4 *V, glm::vec3 *cam_position);

	void PreFirstPass();
	void PostFirstPass();
	void SecondPass();
	//void secondDebug();
};

#endif