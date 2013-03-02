#pragma once
#ifndef TRINIDAD_CAMERA
#define TRINIDAD_CAMERA

#include "Tobago/Tobago.h"

using namespace glm;

class Camera : public Scene {
public:
	mat4 V;
	mat4 *P;
	vec3 position, direction, up;

	Camera(vec3 position, vec3 direction, vec3 up, float vel, bool verbose=false, mat4 *Projection=NULL);

	void update(double t);
	void draw(double t);

private:
	float vel;
	int px, py;
	bool verbose;
	Shader *theShad;
	GLuint MVP_Id;

	void move_left();
	void move_right();
	void move_front();
	void move_back();
	void move_direction(int x, int y);
};

#endif