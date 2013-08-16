#pragma once
#ifndef CORELLA_CAMERA
#define CORELLA_CAMERA

#include "Tobago/Tobago.h"
#include "../Director/SceneManager.h"
#include "../Director/director.h"

using namespace glm;

class Camera : public Scene {
public:
	mat4 P, V;
	vec3 position, direction, up;
	float fov, ratio, znear, zfar;
	float focalLength, focusDistance, FStop;
	int width, height;
	bool debug;

	Camera(int width, int height,									//Screen Info
		   float fov, float znear, float zfar,						//Perspective Info
		   vec3 position, vec3 direction, vec3 up,					//Camera Info
		   float focalLength, float focusDistance, float FStop,		//Lens Info
		   bool debug=false);

	virtual void update(double t);
	virtual void calcMatrix();

private:
	float vel;
	int px, py;
	bool verbose;
	vec2 inf_p;

	void move_left();
	void move_right();
	void move_front();
	void move_back();
	void move_direction(int x, int y);
};

#endif