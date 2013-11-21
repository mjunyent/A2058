#include "Rig.h"

Rig::Rig(const Camera &cam, float eye_sep) : Camera(cam) {
	this->eye_sep = eye_sep;
	calcMatrix();
}

Rig::Rig(int width, int height,										//Screen Info
	float fov, float znear, float zfar,							//Perspective Info
	vec3 position, vec3 direction, vec3 up,	float eye_sep,		//Camera Info
	float focalLength, float focusDistance, float FStop,		//Lens Info
	bool debug) : Camera(width, height,
						 fov, znear, zfar,
						 position, direction, up,
						 focalLength, focusDistance, FStop,
						 debug) {
	 this->eye_sep = eye_sep;
	 calcMatrix();
}

void Rig::calcMatrix() {
	Camera::calcMatrix();

	vec3 axis = normalize(cross(direction, up)); //axis is the vector perpendicular to up & direction, pointing to the left (from where we are looking).

	positionL = position + axis*(eye_sep/2.0f);
	positionR = position - axis*(eye_sep/2.0f);

	center = position + normalize(direction)*focusDistance;

	V_left = lookAt(positionL, center, up);
	V_right = lookAt(positionR, center, up);
}

void Rig::update(double t) {
	Camera::update(t);
	calcMatrix();
}
