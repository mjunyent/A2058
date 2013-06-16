#ifndef SHADERTOY
#define SHADERTOY

#include "Tobago/Tobago.h"

//TODO: Pass Fourier.
class ShaderToy : public Scene {
public:
	Shader *shader;

	ShaderToy(const char *vertex_file, const char *fragment_file);
	void draw(double t);
	void reload(const char *vertex_file, const char *fragment_file);
	VBO *quad;
	IBO *quad_I;

private:
	GLint  globalTime_id;
	GLint  songTime_id;
	GLint  resolution_id;
	GLint  mouse_id;
	GLint  mouse_button_id;

	void load(const char *vertex_file, const char *fragment_file);
};

class ConfRead {
public:
	std::string filename;
	std::string vertex_file, fragment_file, song_file;
	int resX;
	int resY;
	double relTime;

	ConfRead(std::string filname);
	void read();
};

#endif