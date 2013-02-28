#ifndef NO_3DS

#pragma once
#include "../Tobago.h"
#include "lib3ds.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static long fileio_seek_func(void *self, long offset, Lib3dsIoSeek origin);

static long fileio_tell_func(void *self);

static size_t fileio_read_func(void *self, void *buffer, size_t size);

static size_t fileio_write_func(void *self, const void *buffer, size_t size);

static void fileio_log_func(void *self, Lib3dsLogLevel level, int indent, const char *msg);

class A3dsHandler{
public:
	IBO *indexs;
	VBO *vertexs;
	Lib3dsFile *f;
	Lib3dsMesh *mesh;
	Lib3dsFace *faces;

	A3dsHandler(char *filename);
	A3dsHandler(char *filename, int meshid);

private:
	void loadFile(char *filename);
	void makeSimpleVBO(int id);
};

#endif