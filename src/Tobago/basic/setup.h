//
//  setup.h
//  Lynx1
//

#pragma once
#ifndef setup_h
#define setup_h

#include "GL/glew.h"
#include "GL/glfw.h"
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
		int r , int g , int b , 
		int alpha , int depth , int stencil , 
		char *name ,
		int version , int aa , int mode
		 );

}

void GLFWCALL resizecalback( int width, int height );


#endif
