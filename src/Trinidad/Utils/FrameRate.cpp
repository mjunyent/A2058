#include "FrameRate.h"

namespace FrameRate {

//	float fps_box[12];

	float fps_box[] = { 
		-0.99f, 1.0f, 0.0f,	//0 UP, LEFT
		 -0.4f, 1.0f, 0.0f, //1 UP, RIGHT
		 -0.4f, 0.5f, 0.0f, //2 DOWN, RIGHT
		-0.99f, 0.5f, 0.0f  //3 DOWN, LEFT
	};

	GLushort fps_box_I[] = {
		0, 3, 1,
		1, 3, 2
	};

	char fps_text[255];

	Shader *fps_shader;
	FontHandler *fps_font;
	GLuint fps_TXT;

	VBO *fps_texbox;
	VBO *fps_textexbox;
	IBO *fps_texbox_I;

	void setup(int x, int y, int width, int height) {
		fps_box[2] = fps_box[5] = fps_box[8] = fps_box[11] = 0;
		glm::vec2 a(x, y);
		glm::vec2 b(x+width, y);
		glm::vec2 c(x+width, y+height);
		glm::vec2 d(x, y+height);

		cout << a.x << "," << a.y << endl << b.x << "," << b.y << endl << c.x << "," << c.y << endl << d.x << "," << d.y << endl;

		a = pixel2screen(a);
		b = pixel2screen(b);
		c = pixel2screen(c);
		d = pixel2screen(d);

		cout << a.x << "," << a.y << endl << b.x << "," << b.y << endl << c.x << "," << c.y << endl << d.x << "," << d.y << endl;

		fps_box[0] = a.x;
		fps_box[1] = a.y;
		fps_box[3] = b.x;
		fps_box[4] = b.y;
		fps_box[6] = c.x;
		fps_box[7] = c.y;
		fps_box[9] = d.x;
		fps_box[10] = d.y;

		for(int i=0; i<12; i++) {
			cout << fps_box[i] << endl;
		}

		fps_shader = new Shader("rendertext.vert", "rendertext.frag");
		fps_TXT = fps_shader->getUniform("color_tex");

		fps_font = new FontHandler("Calibri.png");

		fps_texbox = new VBO(fps_box, sizeof(fps_box), 0);
		fps_textexbox = new VBO(global::quad, sizeof(global::quad), 1);
		fps_texbox_I = new IBO(fps_box_I, sizeof(fps_box_I));
	}

	void draw(double time) {
		glDisable(GL_DEPTH_TEST);

		sprintf(fps_text, "%.2f fps", 1.0f / global::dt);

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
};
