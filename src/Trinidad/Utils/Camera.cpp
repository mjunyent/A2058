#include "Camera.h"

Camera::Camera(vec3 position, vec3 direction, vec3 up, float vel, bool verbose) {
	this->position = position;
	this->direction = normalize(direction);
	this->up = normalize(up);
	this->vel = vel;

	V = lookAt(position, position+direction, up);

	if(verbose)	glfwEnable(	GLFW_STICKY_KEYS );
	px = 0;
	py = 0;
	this->verbose = verbose;
}

void Camera::update(double t) {
	bool moved = false;

	if(	glfwGetKey( GLFW_KEY_LEFT ) ) {
		moved = true;
		move_left();
	}
	if( glfwGetKey( GLFW_KEY_RIGHT ) ) {
		moved = true;
		move_right();
	}
	if( glfwGetKey( GLFW_KEY_UP ) ) {
		moved = true;
		move_front();
	}
	if( glfwGetKey( GLFW_KEY_DOWN ) ) {
		moved = true;
		move_back();
	}

	int x, y;
	glfwGetMousePos(&x, &y);

	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_1)) {
		move_direction(px-x, py-y);
	}
	px = x;
	py = y;

	V = lookAt(position, position+direction, up);
	if(verbose && moved) std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << endl <<  " Direction: " << direction.x << ", " << direction.y << ", " << direction.z << endl;
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

	v *= (x/float(global::width));
	h *= (y/float(global::height));

	direction += h;
	direction += v;
	direction = normalize(direction);
}
