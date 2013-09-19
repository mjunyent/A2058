#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "../Parser.h"
#include "Cells.h"
#include "Scanner.h"

class Storm : public Scene {
public:
	Shader *billboardShad;
	VBO *singlePoint;
	TBO ballTex;

	FBO *left, *right;

	Camera *myCam;
	Rig *myRig;
	CSParser *csp;

	float quadSize;

	Cells *c;
	Scanner *s;

	//Cells DOF
	Shader *blur;
	VBO *quad;
	IBO *quad_I;
	FBO *renderedCellTemp, *renderedCell;
	float  COCNear, COCFar;
	float blurNearMax, blurFarMax;
	float blurStrength;

	Storm(CSParser *csp);

	void draw(int s, double t);
	void render(int s, double t);

	void renderCell(int i, float cellScreenPositionL, float cellScreenPositionR);
	int getLinearDOFRadius(vec3 p);

	void update(double t);

	void readConf();

private:
	GLint billboard_M_Id, billboard_V_Id, billboard_P_Id,
		  billboard_up_id, billboard_camPos_Id, billboard_r_Id,
		  billboard_tex_Id, billboard_cellPos_Id, billboard_alpha_Id;
	GLint blur_tex_Id, blur_texelSize_Id, blur_orientation_Id, blur_amount_Id, blur_strength_Id;
};

