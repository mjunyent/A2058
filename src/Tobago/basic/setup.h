//
//  setup.h
//  Lynx1
//

#pragma once
#ifndef setup_h
#define setup_h

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "global.h"

#include <iostream>
#include <stdlib.h>

namespace OGL {
	/*init(window width, window height
		red buffer bits, green buffer bits, blue buffer bits,
		alpha buffer bits, depth buffer bits, stencil buffer bits,
		window name,
		OPENGL version,
		Antialising rate
		Window mode);
	*/
    void init(
		int w	, int h , 
		char *name ,
		int major , int minor ,
		int aa , GLFWmonitor *monitor
		 );

}

void resize_callback( GLFWwindow *window, int width, int height );
void error_callback(int error, const char* description);

#endif
