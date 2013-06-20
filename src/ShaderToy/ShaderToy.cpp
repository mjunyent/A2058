#include "ShaderToy.h"

ShaderToy::ShaderToy(const char *vertex_file, const char *fragment_file) {
	glfwEnable(GLFW_STICKY_MOUSE_BUTTONS);
	load(vertex_file, fragment_file);
	quad = new VBO(global::quad, sizeof(global::quad), 0);
	quad_I = new IBO(global::quad_I, sizeof(global::quad_I));
}

void ShaderToy::draw(double time) {
	int x, y, button;
	glfwGetMousePos(&x, &y);
	button = glfwGetMouseButton(GLFW_MOUSE_BUTTON_1);

	shader->use();
	glUniform1f(globalTime_id, global::currentTime);
	glUniform1f(songTime_id, global::song->SoundTime());
	glUniform2f(resolution_id, global::width, global::height);
	glUniform2f(mouse_id, float(x)/global::width, float(y)/global::height);
	glUniform1i(mouse_button_id, button);

	quad->enable(3);
	quad_I->draw(GL_TRIANGLES);
	quad->disable();
}

void ShaderToy::reload(const char *vertex_file, const char *fragment_file) {
	delete(shader);
	load(vertex_file, fragment_file);
}

void ShaderToy::load(const char *vertex_file, const char *fragment_file) {
	shader = new Shader(vertex_file, fragment_file);
	globalTime_id   = shader->getUniform("globalTime");
	songTime_id     = shader->getUniform("songTime");
	resolution_id   = shader->getUniform("res");
	mouse_id	    = shader->getUniform("mouse");
	mouse_button_id = shader->getUniform("button");
}


ConfRead::ConfRead(std::string filename) {
	this->filename = filename;
	read();
}

void ConfRead::read() {
	ifstream fs(filename);

	if(fs.is_open()) {
		fs >> vertex_file >> fragment_file >> song_file;
		fs >> resX >> resY >> relTime;	
	}

	fs.close();
}