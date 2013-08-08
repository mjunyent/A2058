#pragma once
#ifndef TRINIDAD_DEFERRED
#define TRINIDAD_DEFERRED

#include "Tobago/Tobago.h"
#include "../Director/SceneManager.h"
#include "../Director/director.h"
#include "Light.h"

/*
* Corella Deferred Render Engine.
* Just redefine the render function with your models in the Inheritance.
* If you want more control over the render pipeline you can override the draw function.
*/
class Deferred : public Scene {
public: 
	Shader *firstShad;
	Shader *secondShad;
	Shader *thirdShad;
	Shader *debugShad;

	FBO *renderBuffer;
	FBO *finalRender;

	Light *lights;
	glm::mat4 *P, *V;
	glm::vec3 *cam_position;

	VBO *screen_quad;
	IBO *screen_quad_I;

	GLint P_Id, V_Id;
	GLint normalID, diffuseID, specularID, depthID, invPVID, camPosID;
	GLint tex1ID, tex2ID, tex3ID, tex4ID, debInvID;
	GLint finalNID, finalDID, finalID, widthID, heightID;

	GLint bgID;
	TBO bg;

	int width, height;

	int debScreen;

	Deferred(int width, int height, glm::mat4 *P, glm::mat4 *V, glm::vec3 *cam_position, int debScreen=-1);

	virtual void draw(int s, double t);
	virtual void render(int s, double t) {};

	void PreFirstPass();
	void PostFirstPass();
	void SecondPass();
	void ThirdPass();
	void Debug();
};

#endif