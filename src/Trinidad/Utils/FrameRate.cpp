#include "FrameRate.h"

FrameRate::FrameRate(int x, int y, int width, int height) {
	fps_box[2] = fps_box[5] = fps_box[8] = fps_box[11] = 0;
	glm::vec2 a(x, y);
	glm::vec2 b(x+width, y);
	glm::vec2 c(x+width, y+height);
	glm::vec2 d(x, y+height);

	a = pixel2screen(a);
	b = pixel2screen(b);
	c = pixel2screen(c);
	d = pixel2screen(d);

	fps_box[0] = a.x;
	fps_box[1] = a.y;
	fps_box[3] = b.x;
	fps_box[4] = b.y;
	fps_box[6] = c.x;
	fps_box[7] = c.y;
	fps_box[9] = d.x;
	fps_box[10] = d.y;

	fps_shader = new Shader("Shaders/fpsRender.vert", "Shaders/fpsRender.frag");
	fps_TXT = fps_shader->getUniform("color_tex");

	fps_font = new FontHandler("Images/Calibri.png");

	fps_texbox = new VBO(fps_box, sizeof(fps_box), 0);
	fps_textexbox = new VBO(global::quad, sizeof(global::quad), 1);
	fps_texbox_I = new IBO(global::quad_I, sizeof(global::quad_I));

	counter = 0;
	sum_dt = 0.0;
}

void FrameRate::draw(double time) {
	glDisable(GL_DEPTH_TEST);

	TBO fpsTex = fps_font->StringTex(fps_text, strlen(fps_text));

	fps_shader->use();
		fpsTex.bind(0);
		glUniform1i(fps_TXT, 0);
		fps_texbox->enable(3);
		fps_textexbox->enable(3);
		fps_texbox_I->draw(GL_TRIANGLES);
		fps_texbox->disable();
		fps_textexbox->disable();

	glEnable(GL_DEPTH_TEST);
	fpsTex.erase();
}

void FrameRate::update(double time) { //maybe, due to frate function, this doesn't work well, think about it.
	sprintf(fps_text, "%.2f fps", 1.0/global::dt );
/*
	if(counter < 4) {
		sum_dt += global::dt;
		counter++;
	} else {
		sprintf(fps_text, "%.2f fps", 4.0f/sum_dt);
		counter = 0;
		sum_dt = 0;
	}*/
}
