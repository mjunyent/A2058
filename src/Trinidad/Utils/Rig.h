#pragma once
#ifndef TRINIDAD_RIG
#define TRINIDAD_RIG

#include "Tobago/Tobago.h"

using namespace glm;

class Rig : public Scene {
public:
	mat4 V_Left, V_Right;
	vec3 *position, *direction, *up;
	vec3 p_left, p_right, center;
	float eye_sep, center_dist;
	bool debug;

	FBO *left, *right;
	Shader *shad;
	GLuint left_id, right_id;
	VBO *screen_quad;
	IBO *screen_quad_I;

	Rig(vec3 *position, vec3 *direction, vec3 *up, float eye_sep, float center_dist, FBO *left, FBO *right, bool debug=false);

	void draw(double t);

	void update(double t);
 
private:

};


#endif