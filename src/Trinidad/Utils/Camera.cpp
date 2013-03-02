#include "Camera.h"

Camera::Camera(vec3 position, vec3 direction, vec3 up, float vel, bool verbose, mat4 *Projection) {
	this->position = position;
	this->direction = normalize(direction);
	this->up = normalize(up);
	this->vel = vel;

	V = lookAt(position, position+direction, up);

	glfwEnable(	GLFW_STICKY_KEYS );
	px = 0;
	py = 0;
	this->verbose = verbose;

	if(verbose) {
		theShad = new Shader("Shaders/Camera_Line.vert", "Shaders/Camera_Line.frag");
		MVP_Id = theShad->getUniform("MVP");
		P = Projection;
	}
}

void Camera::update(double t) {
	if(	glfwGetKey( GLFW_KEY_LEFT ) ) move_left();
	if( glfwGetKey( GLFW_KEY_RIGHT ) ) move_right();
	if( glfwGetKey( GLFW_KEY_UP ) ) move_front();
	if( glfwGetKey( GLFW_KEY_DOWN ) ) move_back();

	int x, y;
	glfwGetMousePos(&x, &y);

	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_1)) {
		move_direction(px-x, py-y);
	}
	px = x;
	py = y;


	V = lookAt(position, position+direction, up);
	if(verbose) std::cout << position.x << ", " << position.y << ", " << position.z << " - " << direction.x << ", " << direction.y << ", " << direction.z << endl;
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

void Camera::draw(double t) {
/*	if(verbose) {
		std::cout << "asdf" << endl;
		vec3 look = position + direction*50.0f;
		GLfloat theLine[] = {
			position.x, position.y, position.z,
			look.x, look.y, look.z };

		VBO line(theLine, sizeof(theLine), 0);

		mat4 MVP = (*P) * V;

		theShad->use();
			glUniformMatrix4fv(MVP_Id, 1, GL_FALSE, &MVP[0][0]);

			line.enable(3);
			line.draw(GL_LINES);
			line.disable();
			line.destroy();
	}*/
}