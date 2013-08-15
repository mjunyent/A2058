#pragma once
#ifndef CORELLA_RIG
#define CORELLA_RIG

#include "Tobago/Tobago.h"
#include "../Director/SceneManager.h"
#include "Camera.h"

using namespace glm;

class Rig : public Camera {
public:
	mat4 V_left, V_right;
	vec3 positionL, positionR, center;
	float eye_sep;

	Rig(const Camera &cam, float eye_sep);
	//we render the first texture of the FBO.
	Rig(int width, int height,										//Screen Info
		float fov, float znear, float zfar,							//Perspective Info
		vec3 position, vec3 direction, vec3 up,	float eye_sep,		//Camera Info
		float focalLength, float focusDistance, float FStop,		//Lens Info
		bool debug=false);

	void update(double t);
 
private:

};


#endif