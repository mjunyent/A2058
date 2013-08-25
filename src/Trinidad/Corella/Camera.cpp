#include "Camera.h"

Camera::Camera(int width, int height,
			   float fov, float znear, float zfar,
			   vec3 position, vec3 direction, vec3 up,
			   float focalLength, float focusDistance, float FStop,
			   bool debug) {
	this->fov = fov;
	this->width = width;
	this->height = height;
	this->ratio = float(width)/float(height);
	this->znear = znear;
	this->zfar = zfar;

	this->position = position;
	this->direction = normalize(direction);
	this->up = normalize(up);

	this->focalLength = focalLength;
	this->focusDistance = focusDistance;
	this->FStop = FStop;

	this->debug = debug;

	calcMatrix();
}

void Camera::calcMatrix() {
	P = perspective(fov, ratio, znear, zfar);
	V = lookAt(position, position+focusDistance*normalize(direction), up);
}

void Camera::update(double t) {
	calcMatrix();
	//Add movement debug options.
}

void Camera::move_left() {
	vec3 increment = cross(direction, up);
	increment *= vel;
	position = position-increment;
}

void Camera::move_right() {
	vec3 increment = cross(direction, up);
	increment *= vel;
	position = position+increment;
}

void Camera::move_back() {
	position = position - direction*vel;
}

void Camera::move_front() {
	position = position + direction*vel;
}

void Camera::move_direction(int x, int y) {
	vec3 v = cross(direction, up);
	vec3 h = cross(v, direction);

//	v *= (x/DIR float(global::width));
//	h *= (y/float(global::height));

	direction += h;
	direction += v;
	direction = normalize(direction);
}
