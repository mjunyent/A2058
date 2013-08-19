#pragma once
#ifndef TRINIDAD_DEFERRED
#define TRINIDAD_DEFERRED

#include <typeinfo>
#include <string>
#include "Tobago/Tobago.h"
#include "../Director/SceneManager.h"
#include "../Director/director.h"
#include "Light.h"
#include "Camera.h"
#include "Rig.h"

/*
* Corella Deferred Render Engine.
* Just redefine the render function with your models in the Inheritance.
* If you want more control over the render pipeline you can override the draw function.
* PreFirstPass -> Your Geometry (Render) -> PostFirstPass -> renderBuffer
* renderBuffer -> SecondPass -> Screen OR SecondRenderBuff
* SecondRenderBuff -> DOFPass -> Screen OR DOFRenderBuff 
				   -> AAPass -> Screen OR AARenderBuff
* DOFRenderBuff -> AAPass -> Screen OR AARenderBuff
*/
class Deferred : public Scene {
public:
	Shader *firstShad;
	Shader *secondShad;
	Shader *AAShad;
	Shader *DOFShad;
	Shader *debugShad;
	Shader *stereoShad;

	FBO *currentRenderBuffer, *renderBufferL, *renderBufferR;
	FBO *SecondRenderBuff, *DOFTempRenderBuff, *DOFRenderBuff, *AARenderBuff;
	FBO *leftBuff, *rightBuff;

	TBO backgroundTex;

	Light *lights;
	glm::vec3 *bg_color;

	VBO *screen_quad;
	IBO *screen_quad_I;

	glm::mat4 *currentV;
	glm::vec3 *currentCamPos;

	int debScreen;

	Camera *cam;
	Rig    *rig;

	Deferred();
	Deferred(Camera *cam, int debScreen=-1);

	void setBackground(char *name);
	void setBackground(glm::vec3 *color);

	void dotheDOF(bool doit=true);
	void dotheAA(bool doit=true);
	void renderOfscreen(bool doit=true);

	virtual void draw(int s, double t);
	virtual void render(int s, double t) {};

	void doPipeline(int s, double t);
	void doStereoPipeline(int s, double t);

	void PreFirstPass(); //Prepare to launch Geometry.
	void PostFirstPass(); //After Launching Geometry.
	void SecondPass(); //Render quad from FBO.
	void DOFPass(); //Apply DOF.
	void AAPass(); //Apply AA.
	void Debug();

protected:
	void setup(Camera *cam, int debScreen=-1);

	GLint P_Id, V_Id; //First Shad.
	GLint normalID, diffuseID, specularID, depthID, invPVID, camPosID, backgroundID, bgColorID, isBgTexID;
	GLint tex1ID, tex2ID, tex3ID, tex4ID, debInvID; //Debug Shad.
	GLint finalNID, finalDID, finalID, widthID, heightID; //AA Shad.
	GLint DOFTextID, DOFDepthID, DOFBlurCoeffID, DOFFDistID, DOFwidthID, DOFheightID, DOFFarID, DOFNearID, DOFOrientationID;
	GLint StereoLeftID, StereoRightID;

	bool doDOF, doAA, doOffscreen, doStereo;
};

#endif