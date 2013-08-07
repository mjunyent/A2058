#ifndef NO_3DS

#pragma once
#include "../objects/VBO.h"
#include "../objects/IBO.h"
#include "../basic/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>

#include <glm/glm.hpp>
#include <lib3ds.h>


static long fileio_seek_func(void *self, long offset, Lib3dsIoSeek origin);

static long fileio_tell_func(void *self);

static size_t fileio_read_func(void *self, void *buffer, size_t size);

static size_t fileio_write_func(void *self, const void *buffer, size_t size);

static void fileio_log_func(void *self, Lib3dsLogLevel level, int indent, const char *msg);

class A3dsHandler{
public:
	IBO *indexs;
	VBO *vertexs;
	VBO *normals;
	VBO *UVs;
	VBO *tangents;
	VBO *bitangents;
	Lib3dsFile *f;
	Lib3dsMesh *mesh;
	Lib3dsFace *faces;

	float *ndata;

	A3dsHandler(char *filename);
	A3dsHandler(char *filename, int meshid);

	//Per vertex things
	void makeVBOwithIBO(int id);
	void makeNormalsPerVertex();	//1 normal per vertex
	void makeUVs();
	void makeTBNSpace();

	//Per face things
	void makeVBO(int id);			//without IBO, 3 vertex per face.
	void makeNormalsPerFace();		//1 normal per face

	glm::mat4 getModelMatrix();

private:
	void loadFile(char *filename);
};

#endif