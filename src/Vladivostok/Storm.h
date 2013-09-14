#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "Parser.h"
#include "StormAuxiliar.h"

class Storm : public Scene {
public:
	Shader *billboardShad;
	VBO *singlePoint;
	TBO ballTex;

	FBO *left, *right, *current;

	Camera *myCam;
	Rig *myRig;
	CSParser *csp;

	vec3 *currentCamPos;
	mat4 *currentV;
	float quadSize;
	float texSize;

	Cells *c;
	Scanner *s;

	//Cells DOF
	Shader *blur;
	VBO *quad;
	IBO *quad_I;
	FBO *renderedCellTemp, *renderedCell;
	float  COCNear, COCFar;
	float blurNearMax, blurFarMax;
	vec3 LinearDOF; //x: max blur amount, y: near stop, z: far stop.  \y____z/
	float blurStrength;

	Storm(CSParser *csp);

	void draw(int s, double t);
	void render(int s, double t);

	void renderCell(int radius);
	int getLinearDOFRadius(vec3 p);

	void update(double t);

	void readConf();

private:
	GLint billboard_M_Id, billboard_V_Id, billboard_P_Id,
		  billboard_up_id, billboard_camPos_Id, billboard_r_Id,
		  billboard_tex_Id, billboard_texSize_Id, billboard_depth_Id;
	GLint blur_tex_Id, blur_texelSize_Id, blur_orientation_Id, blur_amount_Id, blur_strength_Id;
};

