#pragma once
#ifndef TRINIDAD_FRAMERATE
#define TRINIDAD_FRAMERATE

#include "Tobago/Tobago.h"

using namespace std;

class FrameRate : public Scene {
public:
	FrameRate(int x, int y, int width, int height);
	void draw(double time);

private:
	float fps_box[12];

	Shader *fps_shader;
	FontHandler *fps_font;
	GLuint fps_TXT;

	VBO *fps_texbox;
	VBO *fps_textexbox;
	IBO *fps_texbox_I;
};

#endif
