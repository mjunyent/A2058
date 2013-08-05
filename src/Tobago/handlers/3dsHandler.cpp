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
		TOBAGO::log.write(ERROR) << msg;
	} else if(level == 1) { //Warning
		TOBAGO::log.write(WARNING) << msg;
	}
}

A3dsHandler::A3dsHandler(char *filename) {
	loadFile(filename);
}

A3dsHandler::A3dsHandler(char *filename, int meshid) {
	loadFile(filename);
	makeVBOwithIBO(meshid);
}

void A3dsHandler::loadFile(char *filename) {
	FILE *file;
	Lib3dsIo io;
	int result;

	file = fopen(filename, "rb");
	if(!file) {
		TOBAGO::log.write(ERROR) << "3ds Handler: FILE NOT FOUND " << filename;
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
		TOBAGO::log.write(ERROR) << "Load 3ds file" << filename << " failed!";
		exit(1);
	}
}

void A3dsHandler::makeVBOwithIBO(int id) {
	if(id >= f->nmeshes) {
		TOBAGO::log.write(ERROR) << "Mesh out of bounds!" << id;
		return;
	}

	mesh = f->meshes[id];
	faces = mesh->faces;

	GLfloat *vdata = new float[3*mesh->nvertices];
//	cout << "MESH size: " << mesh->nvertices << endl;

	for(int i=0, e=0; i<3*mesh->nvertices; i+=3, e++) {
		vdata[i]   = mesh->vertices[e][0];
		vdata[i+1] = mesh->vertices[e][1];
		vdata[i+2] = mesh->vertices[e][2];
	}
	vertexs = new VBO(vdata, sizeof(float)*mesh->nvertices*3, 0);

	GLushort *fdata = new GLushort[mesh->nfaces*3];
//	cout << "Number of faces: " << mesh->nfaces << endl;
	for(int i=0, e=0; i<3*mesh->nfaces; i+=3, e++) {
		fdata[i]   = faces[e].index[0];
		fdata[i+1] = faces[e].index[1];
		fdata[i+2] = faces[e].index[2];
	}

	indexs = new IBO(fdata, sizeof(GLushort)*mesh->nfaces*3);
}

void A3dsHandler::makeVBO(int id) {
	if(id >= f->nmeshes) {
		TOBAGO::log.write(ERROR) << "Mesh out of bounds!" << id;
		return;
	}

	mesh = f->meshes[id];
	faces = mesh->faces;

//	cout << "MESH size: " << mesh->nvertices << endl;
//	cout << "Number of faces: " << mesh->nfaces << endl;

	float *vdata = new float[mesh->nfaces*3*3]; //3 vertex per face & 3 coords per vertex;

	int i=0;
	for(int e=0; e<mesh->nfaces; e++) {
		for(int a=0; a<3; a++) {
			vdata[i] = mesh->vertices[faces[e].index[a]][0];
			i++;
			vdata[i] = mesh->vertices[faces[e].index[a]][1];
			i++;
			vdata[i] = mesh->vertices[faces[e].index[a]][2];
			i++;
		}
	}

	vertexs = new VBO(vdata, sizeof(float)*mesh->nfaces*3*3, 0);
}

void A3dsHandler::calculateNormals() {
	//vector of vectors of vec3 (for each vertex we save its normals).
	vector<vector<glm::vec3> > vertex_normals(mesh->nvertices);
	float *ndata = new float[mesh->nvertices*3];

	for(int i=0; i<mesh->nfaces; i++) {
		glm::vec3 v1(mesh->vertices[faces[i].index[0]][0],
					 mesh->vertices[faces[i].index[0]][1],
					 mesh->vertices[faces[i].index[0]][2]);

		glm::vec3 v2(mesh->vertices[faces[i].index[1]][0],
					 mesh->vertices[faces[i].index[1]][1],
					 mesh->vertices[faces[i].index[1]][2]);

		glm::vec3 v3(mesh->vertices[faces[i].index[2]][0],
					 mesh->vertices[faces[i].index[2]][1],
					 mesh->vertices[faces[i].index[2]][2]); 

		glm::vec3 normal = glm::cross(v2-v1, v3-v1);

		vertex_normals[faces[i].index[0]].push_back(normal);
		vertex_normals[faces[i].index[1]].push_back(normal);
		vertex_normals[faces[i].index[2]].push_back(normal);
	}

	for(int i=0; i<mesh->nvertices; i++) {
		glm::vec3 normal(0, 0, 0);

		for(unsigned int j=0; j<vertex_normals[i].size(); j++) {
			normal += vertex_normals[i][j];
		}

		normal = glm::normalize(normal);

		ndata[3*i+0] = normal.x;
		ndata[3*i+1] = normal.y;
		ndata[3*i+2] = normal.z;
	}

	normals = new VBO(ndata, sizeof(float)*mesh->nvertices*3, 1);
}

void A3dsHandler::makeNormals() {
	float (*pre_normals)[3] = (float (*)[3])malloc(sizeof(float) * 9 * mesh->nfaces);
	
	float *ndata = new float[3*3*mesh->nfaces];
	lib3ds_mesh_calculate_vertex_normals(mesh, pre_normals);

	/* To access the normal of the i-th vertex of the j-th face do the
	* following:
	* \code
	*   normals[3*j+i]
	* \endcode
	*/
	int i=0;
	for(int e=0; e<mesh->nfaces; e++) {
		for(int a=0; a<3; a++) { 
			ndata[i] = pre_normals[e*3+a][0];
			i++;
			ndata[i] = pre_normals[e*3+a][1];
			i++;
			ndata[i] = pre_normals[e*3+a][2];
			i++;
		}
	}

	normals = new VBO(ndata, sizeof(float)*mesh->nfaces*3*3, 1);

	/*
	int i=0;
	for(int e=0; e<mesh->nfaces; e++) {
		glm::vec3 p1(mesh->vertices[faces[e].index[0]][0], mesh->vertices[faces[e].index[0]][1], mesh->vertices[faces[e].index[0]][2]);
		glm::vec3 p2(mesh->vertices[faces[e].index[1]][0], mesh->vertices[faces[e].index[1]][1], mesh->vertices[faces[e].index[1]][2]);
		glm::vec3 p3(mesh->vertices[faces[e].index[2]][0], mesh->vertices[faces[e].index[2]][1], mesh->vertices[faces[e].index[2]][2]);

		glm::vec3 normal = glm::normalize(glm::cross(p2-p1, p3-p1));

		for(int a=0; a<3; a++) {
			ndata[i] = normal.x;
			i++;
			ndata[i] = normal.y;
			i++;
			ndata[i] = normal.z;
			i++;
		}
	}*/
}

#endif