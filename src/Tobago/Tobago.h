#include <GL/glew.h> //OPENGL Libs...
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> //GLM Libs...
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>


#ifdef GLEWLIB
	#pragma comment(lib, "glew32s.lib");
#endif

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include "basic/log.h" //simple log class.

#include "basic/basic.h" //functions to change coordinates sistem (pixel, screen or normalized).
#include "basic/setup.h" //functions to initialize the screen and the stuff.

#include "objects/Buffer_Object.h" //abstract class to manage buffer objects.
#include "objects/FBO.h" //framebuffer object class.
#include "objects/IBO.h" //index buffer object class.
#include "objects/Shader.h" //Shader from file manager class.
#include "objects/TBO.h" //Texture buffer object class.
#include "objects/VBO.h" //Vertex buffer object class.

#include "handlers/FontHandler.h" //Font manager and text renderer.
#include "handlers/SoundHandler.h" //Sound manager, loads and plays a song, includes FFT.
#include "handlers/3dsHandler.h" //3ds file importer, generates VBOs and IBOs.
