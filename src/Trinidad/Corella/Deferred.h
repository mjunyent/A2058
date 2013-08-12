#pragma once
#ifndef TRINIDAD_DEFERRED
#define TRINIDAD_DEFERRED

#include "Tobago/Tobago.h"
#include "../Director/SceneManager.h"
#include "../Director/director.h"
#include "Light.h"
#include "Camera.h"

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

	TBO backgroundTex;

	Light *lights;
	glm::vec3 *bg_color;

	VBO *screen_quad;
	IBO *screen_quad_I;

	GLint P_Id, V_Id; //First Shad.
	GLint normalID, diffuseID, specularID, depthID, invPVID, camPosID, backgroundID, bgColorID, isBgTexID;
	GLint tex1ID, tex2ID, tex3ID, tex4ID, debInvID; //Debug Shad.
	GLint finalNID, finalDID, finalID, widthID, heightID; //AA Shad.

	int debScreen;

	Camera *cam;

	Deferred(Camera *cam, int debScreen=-1);

	void setBackground(char *name);
	void setBackground(glm::vec3 *color);

	virtual void draw(int s, double t);
	virtual void render(int s, double t) {};

	void PreFirstPass(); //Prepare to launch Geometry
	void PostFirstPass(); //After Launching Geometry
	void SecondPass(); //Render quad from FBO
	void ThirdPass(); //Apply AA.
	void Debug();

protected:
	void setup();
};

#endif