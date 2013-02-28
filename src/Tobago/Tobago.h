#include "GL/glew.h"
#include "GL/glfw.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"


#ifdef GLEWLIB
	#pragma comment(lib, "glew32s.lib");
#endif

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

#include "basic/basic.h"
#include "basic/setup.h"
#include "basic/global.h"
#include "basic/log.h"
#include "basic/timer.h"

#include "objects/Buffer_Object.h"
#include "objects/FBO.h"
#include "objects/IBO.h"
#include "objects/Shader.h"
#include "objects/SShader.h"
#include "objects/TBO.h"
#include "objects/VBO.h"

#include "handlers/FontHandler.h"
#include "handlers/SceneManager.h"
#include "handlers/SoundHandler.h"
#include "handlers/3dsHandler.h"
