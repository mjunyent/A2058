#include "Shader.h"

Shader::Shader(const char *vertex, const char *fragment) {
    load(vertex, NULL, fragment);
    compile();
    link();
}

Shader::Shader(const char *vertex, const char *fragment, bool verbose) {
    loadmemory(vertex, fragment);
    compile();
    link();
}

Shader::Shader(const char *vertex, const char *geometry, const char *fragment) {
	load(vertex, geometry, fragment);
    compile();
    link();
}

bool Shader::load(const char *vertex, const char *geometry, const char *fragment) {
	bool ret = true;
	v = g = f = -1;

	if(vertex != NULL) {
		char *vs;
		v = glCreateShader(GL_VERTEX_SHADER);

		//Loading vertex shader.
		vs = textFileRead(vertex);
		if (vs == NULL) {
			TOBAGO::log.write(ERROR) << "Vertex shader wasn't read correctly: " << vertex;
		    ret = false;
	    } else {
			vv = vs;
			if(ret) glShaderSource(v, 1, &vv, NULL);
			free(vs);
		}
	}

	if(geometry != NULL) {
		char *gs;
		g = glCreateShader(GL_GEOMETRY_SHADER);

		//Loading geometry shader.
		gs = textFileRead(geometry);
		if (gs == NULL) {
			TOBAGO::log.write(ERROR) << "Geometry shader wasn't read correctly: " << geometry;
			ret = false;
		} else {
			gg = gs;
			if(ret) glShaderSource(g, 1, &gg, NULL);
			free(gs);
		}
	}

	if(fragment != NULL) {
		char *fs;
	    f = glCreateShader(GL_FRAGMENT_SHADER);

		//Loading fragment shader.
	    fs = textFileRead(fragment);
	    if(fs == NULL) {
			TOBAGO::log.write(ERROR) << "Fragment shader file wasn't read correctly:" << fragment;
	        ret = false;
	    } else {
		    ff = fs;
			if(ret) glShaderSource(f, 1, &ff, NULL);
			free(fs);
		}
	}

    return ret;
}

void Shader::loadmemory(const char *vertex, const char *fragment) {
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    //Loading vertex shader.
    glShaderSource(v, 1, &vertex, NULL);

    //Loading fragment shader.
	glShaderSource(f, 1, &fragment, NULL);
}

bool Shader::compile() {
	bool cv, cg, cf;
	cv = cg = cf = false;

	if(v != -1) {
		glCompileShader(v);
		cv = printShaderInfoLog(v); //returns true if error
	}
	if(g != -1) {
		glCompileShader(g);
	    cg = printShaderInfoLog(g); //returns true if error
	}
	if(f != -1) {
		glCompileShader(f);
		cf = printShaderInfoLog(f); //returns true if error
	}
	
	return !(cv || cg || cf); //returns false if error		
}

bool Shader::link() {
    p = glCreateProgram();

	if(v != -1)    glAttachShader(p, v);
	if(g != -1)	   glAttachShader(p, g);
	if(f != -1)    glAttachShader(p, f);

    glLinkProgram(p);
    bool pl = printProgramInfoLog(p); //returns true if error

    return !pl;
}

void Shader::use() {
    glUseProgram(p);
}

GLint Shader::getUniform(const char *name) {
	return glGetUniformLocation(p, name);
}

/*void Shader::unuse() {
    glUseProgram(0);
}*/
//El shader 0 o shader trivial no hace falta llamarlo.


void Shader::uni_float(char *name, float value) {
    GLint loc = glGetUniformLocation(p, name);
    glUniform1f(loc, value);
}

void Shader::uni_int(char *name, int value) {
    GLint loc = glGetUniformLocation(p, name);
    glUniform1i(loc, value);
}

/*
void setShaders() {
    GLint loc = glGetUniformLocation(p, "tex");
    
    glUniform1i(loc, 0);
}*/


char *Shader::textFileRead(const char *fn) {
    FILE *fp;
    char *content = NULL;
    
    int count=0;

    if (fn != NULL) {
        fp = fopen(fn,"rt");

        if (fp != NULL) {
            fseek(fp, 0, SEEK_END);
            count = int(ftell(fp));
            rewind(fp);
            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = int(fread(content,sizeof(char),count,fp));
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

bool Shader::printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	
    if (infologLength > 1)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		TOBAGO::log.write(ERROR) << "Shader compiling error:" << infoLog;
        free(infoLog);
    }

    return !(infologLength > 1);
}

bool Shader::printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    
    if (infologLength > 1)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		TOBAGO::log.write(ERROR) << "Shader program linking error:" << infoLog;
        free(infoLog);
    }

    return !(infologLength > 1);
}
