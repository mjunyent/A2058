#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "Parser.h"

struct Cell
{
	vec3 p;
	vec3 v;
};

struct DepthSort
{
    bool operator()( const Cell& lx, const Cell& rx ) const {
    	return lx.p.z < rx.p.z;
    }
};

class Cells {
public:
	vector<Cell> cells;
	float xRange, yRange, zNear, zFar, zFarAway;
	float vel;

	float K, M, L;

	Cells(int n, float v, float xRange, float yRange, float zNear, float zFar, float zFarAway, float K, float L, float M);

	void update();
};

class Storm : public Scene {
public:
	Shader *billboardShad;
	VBO *singlePoint;
	TBO ballTex, bgTex;

	FBO *left, *right;

	Camera *myCam;
	Rig *myRig;
	CSParser *csp;

	vec3 *currentCamPos;
	mat4 *currentV;
	float quadSize;
	float texSize;

	Cells *c;

	Storm(CSParser *csp);

	void draw(int s, double t);
	void render(int s, double t);

	void update(double t);

private:
	GLint billboard_M_Id, billboard_V_Id, billboard_P_Id,
		  billboard_up_id, billboard_camPos_Id, billboard_r_Id,
		  billboard_tex_Id, billboard_texSize_Id, billboard_bgTex_Id,
		  billboard_depth_Id;
};

