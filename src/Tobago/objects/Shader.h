//
//  shader.h
//  Lynx1
//

#pragma once
#ifndef shader_h
#define shader_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../utility/log.h"
#include <cstdio>
#include <cstdlib>

class Shader {
public:
    Shader() { };
	//From file
    Shader(const char *vertex, const char *fragment); //load, compile and link the shaders at one call.
	Shader(const char *vertex, const char *geometry, const char *fragment);
	//From memory
	Shader(const char *vertex, const char *fragment, bool verbose); //load, compile and link the shaders LOAD FROM MEMORY, VERBOSE NOT IMPLEMENTED.

	bool load(const char *vertex, const char *geometry, const char *fragment); //returns true if everything is ok.
	void loadmemory(const char *vertex, const char *fragment);
    bool compile(); //returns true if everything ok.
    bool link(); //returns true if everything ok.
	//void getV(GLuint BID , char *name_t);
    void use();
    //void unuse();
	GLint getUniform(const char *name);

    void uni_float(char *name, float value);
    void uni_int(char *name, int value);
	GLuint p;

private:
    GLint v, g, f; //this should be glUint but, meh.
    const char *vv, *gg, *ff;
    char *textFileRead(const char *fn);
    bool printShaderInfoLog(GLuint obj); //returns true if error.
    bool printProgramInfoLog(GLuint obj); //returns true if error.
};

#endif
