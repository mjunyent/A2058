#include "Camera.h"

Camera::Camera(float fov, int width, int height, float znear, float zfar, vec3 position, vec3 direction, vec3 up, bool debug) {
	this->fov = fov;
	this->width = width;
	this->height = height;
	this->ratio = float(width)/float(height);
	this->znear = znear;
	this->zfar = zfar;

	this->position = position;
	this->direction = normalize(direction);
	this->up = normalize(up);

	this->debug = debug;

	P = perspective(fov, ratio, znear, zfar);
	V = lookAt(position, position+direction, up);
}

void Camera::update(double t) {
	P = perspective(fov, ratio, znear, zfar);
	V = lookAt(position, position+direction, up);
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
