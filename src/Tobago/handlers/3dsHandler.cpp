#ifndef NO_3DS

#include "3dsHandler.h"

static long
fileio_seek_func(void *self, long offset, Lib3dsIoSeek origin) {
    FILE *f = (FILE*)self;
    int o;
    switch (origin) {
        case LIB3DS_SEEK_SET:
            o = SEEK_SET;
            break;

        case LIB3DS_SEEK_CUR:
            o = SEEK_CUR;
            break;

        case LIB3DS_SEEK_END:
            o = SEEK_END;
            break;
    }
    return (fseek(f, offset, o));
}


static long
fileio_tell_func(void *self) {
    FILE *f = (FILE*)self;
    return(ftell(f));
}


static size_t
fileio_read_func(void *self, void *buffer, size_t size) {
    FILE *f = (FILE*)self;
    return (fread(buffer, 1, size, f));
}


static size_t
fileio_write_func(void *self, const void *buffer, size_t size) {
    FILE *f = (FILE*)self;
    return (fwrite(buffer, 1, size, f));
}

static void 
fileio_log_func(void *self, Lib3dsLogLevel level, int indent, const char *msg)
{
    //    "ERROR", "WARN", "INFO", "DEBUG" (info and debug not displayed).
	if(level == 0) { //Error
		global::log.error(msg);
	} else if(level == 1) { //Warning
		global::log.warning(msg);
	}
}

A3dsHandler::A3dsHandler(char *filename) {
	loadFile(filename);
}

A3dsHandler::A3dsHandler(char *filename, int meshid) {
	loadFile(filename);
	makeSimpleVBO(meshid);
}


void A3dsHandler::loadFile(char *filename) {
	FILE *file;
	Lib3dsIo io;
	int result;

	file = fopen(filename, "rb");
	if(!file) {
		global::log.error("3ds Handler FILE NOT FOUND");
		exit(1);
	}

	f = lib3ds_file_new();

	memset(&io, 0, sizeof(io));
	io.self = file;
	io.seek_func = fileio_seek_func;
    io.tell_func = fileio_tell_func;
    io.read_func = fileio_read_func;
    io.write_func = fileio_write_func;
    io.log_func = fileio_log_func;

	result = lib3ds_file_read(f, &io);

	fclose(file);

	if(!result) {
		global::log.error("Load 3ds file failed");
		exit(1);
	}
}

void A3dsHandler::makeSimpleVBO(int id) {
	if(id >= f->nmeshes) {
		global::log.error("Mesh out of bounds!");
		exit(1);
	}

	mesh = f->meshes[id];
	faces = mesh->faces;

	float *vdata = new float[3*mesh->nvertices];

	for(int i=0, e=0; i<3*mesh->nvertices; i+=3, e++) {
		vdata[i]   = mesh->vertices[e][0];
		vdata[i+1] = mesh->vertices[e][1];
		vdata[i+2] = mesh->vertices[e][2];
	}
	vertexs = new VBO(vdata, sizeof(vdata), 0);

	delete(vdata);

	
	GLushort *fdata = new GLushort[mesh->nfaces*3];

	for(int i=0, e=0; i<3*mesh->nfaces; i+=3, e++) {
		fdata[i]   = faces[e].index[0];
		fdata[i+1] = faces[e].index[1];
		fdata[i+2] = faces[e].index[2];
	}

	indexs = new IBO(fdata, sizeof(fdata));
}

#endif