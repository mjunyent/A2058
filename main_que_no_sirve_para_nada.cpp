#include "GL/glew.h"
#include "GL/glfw.h"

#ifdef GLEWLIB
#pragma comment(lib,"glew32s.lib")
#endif

#ifdef DEBUG_LOG
#include <iostream>
#include "basic/log.h"
#else
#include "basic/shader_string.h" //PLEASE: WHILE DEBUGGING USE BASIC SHADER CLASS (CONFIGURE YOUR DEBUG FOLDER)
#endif

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include "basic/setup.h"
#include "basic/shader.h"
#include "basic/global.h"
//#include "basic/geostuff.hpp"
#include "basic/timer.h"
//#include "basic/textures.h"
#include "basic/FBO.h"
#include "basic/TBO.h"

int main()
{
	//init windows ang ogl and everything.
	OGL::init(1024,768,0,0,0,32,64,1,"our little little sandbox",3,4,GLFW_WINDOW);



//Carga de shaders//
	//Shader para objeto.
	//load compile and link shader.
	//Shader test("Shaders/test.vert", "Shaders/test.frag");
	//load timer class
	//Timer timer(update_M);
	//Get from shader the fucking uniform locations (shader.p is program id)
	//GLuint Ctest = glGetUniformLocation(test.p,"col");
	//GLuint MVPtest = glGetUniformLocation(test.p,"MVP");

	//Shader para quad.
	//Shader quad("Shaders/Passthrough.vert", "Shaders/Passthrough.frag");
	//GLuint texID = glGetUniformLocation(quad.p, "renderedTexture");



	//Creating VBO's
	/*GLuint Cubebuffer;
	glGenBuffers(1, &Cubebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, Cubebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_wtf), cube_wtf, GL_STATIC_DRAW);

	GLuint Quadbuffer;
	glGenBuffers(1, &Quadbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, Quadbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_wtf), quad_wtf, GL_STATIC_DRAW);
	*/






	//Habilita el culling (para más info preguntar a marc que es)
	//glEnable(GL_CULL_FACE);

	glClearColor(0.11f , 0.11f , 0.11f , 0.11f );


	//CODIGO ANTIGUO USANDO VBO CLASS

	//Hoygan, hay que hacer siempre dos VBO para un objeto? No entiendo bien como va y la clase me parece infumable.
	//Un dia de estos lo hablamos sergio y me cuentas bien como va todo. (y luego tampoco entiendo como lo hace para pasare la mierda al shader o loquesea que hace (en el bucle)).
	//VBO tal( cube , sizeof(cube) );
	//VBO pascual( cube_I , sizeof(cube_I) );

	//Si declaro aqui el shader peta cosa mala.
	//Shader quad("Shaders/Passthrough.vert", "Shaders/quad.frag");

	//VBO del QUAD de la pantalla. HOYGAN SI CREO AQUI OTRO VBO PETA COSA MALA
	//VBO tal_quad( quad, sizeof(quad) );
	//VBO pascual_quad( quad_I, sizeof(quad_I) );



	// MIERDA DE FBO //

	// ---------------------------------------------
	// Render to Texture - specific code begins here
	// ---------------------------------------------
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	TBO textmdp(GL_RGBA, 1024, 768, GL_RGBA, GL_UNSIGNED_BYTE, 0, false);

	// The texture we're going to render to
/*	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 1024, 768, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
	//AQUI NO HAY ALPHA, pero en el SHADER hay que devolver alpha, sino peta.


	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/

	// The depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textmdp.theID, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0,GL_DEPTH_ATTACHMENT};
	glDrawBuffers(2, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	return false;





	FBO ftest(1024, 768);

	if(ftest.status == false) global::log.error("COULD NOT CREATE FBO MAN!");
	else global::log.error("asdf");

	while( 1 )
	{
		// Render to our framebuffer
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		//glViewport(0,0,1024,768); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		ftest.bind();

		glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
		test.use();

		//Give shader the values of uniforms
		glUniformMatrix4fv(MVPtest,1,GL_FALSE,&MVP[0][0]);
		glUniform3fv(Ctest,1,&octhdrn_C[1][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, Cubebuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 36); // From index 0 to 3 -> 1 triangle

		glDisableVertexAttribArray(0);


	//COSAS DE LOS VBOS
	//	tal.POINTER(0,3,GL_FALSE,0);
	//	pascual.DRAW(GL_TRIANGLES,0,36);

	//	tal.DISABLE();

		ftest.unbind();

//		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		quad.use();


		ftest.bind_texture(0);

		// Bind our texture in Texture Unit 0
//		textmdp.bind(0);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		// Set our "renderedTexture" sampler to user Texture Unit 0
		glUniform1i(texID, 0);


		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, Quadbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 6); // From index 0 to 3 -> 1 triangle

		glDisableVertexAttribArray(0);

		glfwSwapBuffers();
		timer.update();
	}
	glfwTerminate();
	return 0;
}
