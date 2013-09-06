#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "Parser.h"

struct DepthSort
{
    bool operator()( const vec3& lx, const vec3& rx ) const {
    	return lx.z < rx.z;
    }
};

class Storm : public Scene {
public:
	Shader *billboardShad;
	VBO *singlePoint;
	TBO ballTex;

	FBO *left, *right;

	Camera *myCam;
	Rig *myRig;
	CSParser *csp;

	vec3 *currentCamPos;
	mat4 *currentV;
	vector<vec3> M_ball;
	float quadSize;

	Storm(CSParser *csp);

	void draw(int s, double t);
	void render(int s, double t);

	void update(double t);

private:
	GLint billboard_M_Id, billboard_V_Id, billboard_P_Id, billboard_up_id, billboard_camPos_Id, billboard_r_Id, billboard_tex_Id;
};