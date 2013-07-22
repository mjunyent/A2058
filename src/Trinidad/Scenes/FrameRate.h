#pragma once
#ifndef TRINIDAD_FRAMERATE
#define TRINIDAD_FRAMERATE

#include "Tobago/Tobago.h"
#include "../Director/SceneManager.h"

using namespace std;

class FrameRate : public Scene {
public:
	FrameRate(int x, int y, int width, int height);
	void draw(int s, double time);
	void update(double time);

private:
	char fps_text[255];
	float fps_box[12];
	int counter;
	double sum_dt;

	Shader *fps_shader;
	FontHandler *fps_font;
	GLuint fps_TXT;

	VBO *fps_texbox;
	VBO *fps_textexbox;
	IBO *fps_texbox_I;
};

#endif
