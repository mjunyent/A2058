#include "Rig.h"

Rig::Rig(vec3 *position, vec3 *direction, vec3 *up, float *eye_sep, float *center_dist, FBO *left, FBO *right, THREEDMODES mode, bool debug) {
	this->position = position;
	this->direction = direction;
	this->up = up;
	this->eye_sep = eye_sep;
	this->center_dist = center_dist;

	this->left = left;
	this->right = right;

	this->debug = debug;

	if(mode == SIDEBYSIDE)   shad = new Shader("Shaders/ScreenTexture.vert", "Shaders/3D/SidebySide.frag");
	else if(mode == UPNDOWN) shad = new Shader("Shaders/ScreenTexture.vert", "Shaders/3D/UpnDown.frag");
	else                     shad = new Shader("Shaders/ScreenTexture.vert", "Shaders/3D/AnaglyphRC.frag");

	left_id  = shad->getUniform("LeftTex");
	right_id = shad->getUniform("RightTex");

	screen_quad = new VBO(global::quad, sizeof(global::quad), 0);
	screen_quad_I  = new IBO(global::quad_I, sizeof(global::quad_I));

//	if(debug) glfwEnable( GLFW_STICKY_KEYS );
}

void Rig::draw(double t) {
	glDisable(GL_DEPTH_TEST);
	shad->use();

	left->bind_texture(0, 0);
	right->bind_texture(0, 1);
	glUniform1i(left_id, 0);
	glUniform1i(right_id, 1);

	screen_quad->enable(3);
	screen_quad_I->draw(GL_TRIANGLES);
	screen_quad->disable();
	
	glEnable(GL_DEPTH_TEST);
}

void Rig::update(double t) {
	if(debug) {
		bool print = false;
		if(	glfwGetKey(global::MainWindow, 'A' ) ) {
			*eye_sep -= 0.005f;
			print = true;
		}
		if(	glfwGetKey(global::MainWindow, 'D' ) ) {
			*eye_sep += 0.005f;
			print = true;
		}
		if(	glfwGetKey(global::MainWindow, 'W' ) ) {
			*center_dist += 0.02f;
			print = true;
		}
		if(	glfwGetKey(global::MainWindow, 'S' ) ) {
			*center_dist -= 0.02f;
			print = true;
		}

		if( glfwGetKey(global::MainWindow, 'P' ) ) {
			ofstream of("THREEDMAAAN.txt");

			of << "Eye_Sep        Center_dist" << endl;
			of << *eye_sep << "        " << *center_dist;

			of.close();
		}

		if(print) {
			vec3 ctr = *position + *direction*(*center_dist);
			cout << "Eye sep: " << *eye_sep << ", Center dist: " << *center_dist << ", Center: " << ctr.x << "," << ctr.y << "," << ctr.z << endl;
		}
	}

	vec3 axis = normalize(cross(*up, *direction)); //axis is the vector perpendicular to up & direction, pointing to the left (from where we are looking).

	p_left  = *position + axis*(*eye_sep/2.0f);
	p_right = *position - axis*(*eye_sep/2.0f);

	center = *position + *direction*(*center_dist);

	V_Left = lookAt(p_left, center, *up);
	V_Right = lookAt(p_right, center, *up);
}
