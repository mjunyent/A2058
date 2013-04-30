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

	if(debug) glfwEnable( GLFW_STICKY_KEYS );
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
		if(	glfwGetKey( 'A' ) ) {
			*eye_sep -= 0.005;
			print = true;
		}
		if(	glfwGetKey( 'D' ) ) {
			*eye_sep += 0.005;
			print = true;
		}
		if(	glfwGetKey( 'W' ) ) {
			*center_dist += 0.02;
			print = true;
		}
		if(	glfwGetKey( 'S' ) ) {
			*center_dist -= 0.02;
			print = true;
		}

		if(print) {
			vec3 ctr = *position + *direction*(*center_dist);
			cout << "Eye sep: " << *eye_sep << ", Center dist: " << *center_dist << ", Center: " << ctr.x << "," << ctr.y << "," << ctr.z << endl;

			cout << V_Left[0][0] << " " << V_Left[0][1] << " " << V_Left[0][2] << " " << V_Left[0][3] << endl;
			cout << V_Left[1][0] << " " << V_Left[1][1] << " " << V_Left[1][2] << " " << V_Left[1][3] << endl;
			cout << V_Left[2][0] << " " << V_Left[2][1] << " " << V_Left[2][2] << " " << V_Left[2][3] << endl;
			cout << V_Left[3][0] << " " << V_Left[3][1] << " " << V_Left[3][2] << " " << V_Left[3][3] << endl;
			cout << endl;
			cout << V_Right[0][0] << " " << V_Right[0][1] << " " << V_Right[0][2] << " " << V_Right[0][3] << endl;
			cout << V_Right[1][0] << " " << V_Right[1][1] << " " << V_Right[1][2] << " " << V_Right[1][3] << endl;
			cout << V_Right[2][0] << " " << V_Right[2][1] << " " << V_Right[2][2] << " " << V_Right[2][3] << endl;
			cout << V_Right[3][0] << " " << V_Right[3][1] << " " << V_Right[3][2] << " " << V_Right[3][3] << endl;
		}
	}

	vec3 axis = normalize(cross(*up, *direction)); //axis is the vector perpendicular to up & direction, pointing to the left (from where we are looking).

	p_left  = *position + axis*(*eye_sep/2.0f);
	p_right = *position - axis*(*eye_sep/2.0f);

	center = *position + *direction*(*center_dist);

	V_Left = lookAt(p_left, center, *up);
	V_Right = lookAt(p_right, center, *up);
}
