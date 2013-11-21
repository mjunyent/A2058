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
		TOBAGO::log.write(ERROR) << "3dsHandler error when reading: " << msg;
	} else if(level == 1) { //Warning
		TOBAGO::log.write(WARNING) << "3dsHandler error when reading: " << msg;
	}
}

A3dsHandler::A3dsHandler(char *filename) {
	indexs = NULL;
	vertexs = NULL;
	normals = NULL;
	UVs = NULL;
	tangents = NULL;
	bitangents = NULL;

	loadFile(filename);
}

A3dsHandler::A3dsHandler(char *filename, int meshid) {
	indexs = NULL;
	vertexs = NULL;
	normals = NULL;
	UVs = NULL;
	tangents = NULL;
	bitangents = NULL;

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

void A3dsHandler::makeIndexsByMaterial() {
	indexsByMaterial = vector<IBO*>(f->nmaterials);

	GLushort *fdata = new GLushort[mesh->nfaces*3];

	for(int i=0; i<f->nmaterials; i++) {
		int size = 0;
		for(int j=0, e=0; j<3*mesh->nfaces; j+=3, e++) {
			if(faces[e].material == i) {
				size += 3;
				fdata[j]   = faces[e].index[0];
				fdata[j+1] = faces[e].index[1];
				fdata[j+2] = faces[e].index[2];
			}
		}

		indexsByMaterial[i] = new IBO(fdata, sizeof(GLushort)*size);
	}
}

void A3dsHandler::makeVBO(int id, int material) {
	if(id >= f->nmeshes) {
		TOBAGO::log.write(ERROR) << "Mesh out of bounds!" << id;
		return;
	}

	mesh = f->meshes[id];
	faces = mesh->faces;

	float *vdata = new float[mesh->nfaces*3*3]; //3 vertex per face & 3 coords per vertex;
	GLushort *idata = new GLushort[mesh->nfaces*3];

	int nMfaces = 0;
	int i=0;
	for(int e=0; e<mesh->nfaces; e++) {
		if(faces[e].material == material) {
			nMfaces++;
			for(int a=0; a<3; a++) {
				vdata[i] = mesh->vertices[faces[e].index[a]][0];
				i++;
				vdata[i] = mesh->vertices[faces[e].index[a]][1];
				i++;
				vdata[i] = mesh->vertices[faces[e].index[a]][2];
				i++;
			}
		}
	}

	vertexs = new VBO(vdata, sizeof(float)*nMfaces*3*3, 0);

	for(int k=0; k<nMfaces*3; k++) {
		idata[k] = k;
	}

	indexs = new IBO(idata, sizeof(GLushort)*nMfaces*3);

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
	GLushort *idata = new GLushort[mesh->nfaces*3];

	for(int k=0; k<mesh->nfaces*3; k++) {
		idata[k] = k;
	}

	indexs = new IBO(idata, sizeof(GLushort)*mesh->nfaces*3);

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

void A3dsHandler::makeFuckingNormals() {

	for(int i=0; i<mesh->nvertices; i++) {
		for(int j=i+1; j<mesh->nvertices; j++) {
			if(mesh->vertices[i][0] == mesh->vertices[j][0] &&
			   mesh->vertices[i][1] == mesh->vertices[j][1] &&
			   mesh->vertices[i][2] == mesh->vertices[j][2])
			   TOBAGO::log.write(DEBUG) << "Vertex " << i << " equals " << j << endl <<
			                 ndata[3*i+0] << " " << ndata[3*i+1] << " " << ndata[3*i+2] << endl <<
							 ndata[3*j+0] << " " << ndata[3*j+1] << " " << ndata[3*j+2];
		}
	}

/*	float (*pre_normals)[3] = (float (*)[3])malloc(sizeof(float) * 9 * mesh->nfaces);

	ndata = new float[mesh->nvertices*3];
	for(int i=0; i<mesh->nvertices*3; i++) ndata[i] = 0.0f;

	lib3ds_mesh_calculate_vertex_normals(mesh, pre_normals);

	for(int i=0; i<mesh->nfaces; i++) {
		ndata[3*faces[i].index[0]+0] = pre_normals[3*i+0][0];
		ndata[3*faces[i].index[0]+1] = pre_normals[3*i+0][1];
		ndata[3*faces[i].index[0]+2] = pre_normals[3*i+0][2];

		ndata[3*faces[i].index[1]+0] = pre_normals[3*i+1][0];
		ndata[3*faces[i].index[1]+1] = pre_normals[3*i+1][1];
		ndata[3*faces[i].index[1]+2] = pre_normals[3*i+1][2];

		ndata[3*faces[i].index[2]+0] = pre_normals[3*i+2][0];
		ndata[3*faces[i].index[2]+1] = pre_normals[3*i+2][1];
		ndata[3*faces[i].index[2]+2] = pre_normals[3*i+2][2];
	}

	normals = new VBO(ndata, sizeof(float)*mesh->nvertices*3, 1);*/
}

void A3dsHandler::makeNormalsPerVertex() {
	//vector of vectors of vec3 (for each vertex we save its normals).
	ndata = new float[mesh->nvertices*3];

	for(int i=0; i<mesh->nvertices; i++) {
		glm::vec3 normal(0,0,0);

		for(int j=0; j<mesh->nfaces; j++) {
			if( (mesh->vertices[faces[j].index[0]][0] == mesh->vertices[i][0] && mesh->vertices[faces[j].index[0]][1] == mesh->vertices[i][1] && mesh->vertices[faces[j].index[0]][2] == mesh->vertices[i][2]) ||
			    (mesh->vertices[faces[j].index[1]][0] == mesh->vertices[i][0] && mesh->vertices[faces[j].index[1]][1] == mesh->vertices[i][1] && mesh->vertices[faces[j].index[1]][2] == mesh->vertices[i][2]) ||
			    (mesh->vertices[faces[j].index[2]][0] == mesh->vertices[i][0] && mesh->vertices[faces[j].index[2]][1] == mesh->vertices[i][1] && mesh->vertices[faces[j].index[2]][2] == mesh->vertices[i][2]) ) {
				   normal += calcFaceNormal(j);
			}
		}

		normal = glm::normalize(normal);
		
//		TOBAGO::log.write(DEBUG) << "Normal: " << normal.x << ", " << normal.y << ", " << normal.z;

		ndata[3*i+0] = normal.x;
		ndata[3*i+1] = normal.y;
		ndata[3*i+2] = normal.z;
	}

	normals = new VBO(ndata, sizeof(float)*mesh->nvertices*3, 1);
}

void A3dsHandler::saveNormalsToFile(const char* filename) {
	ofstream output;
	output.open(filename);

	for(int i=0; i<mesh->nvertices; i++) {
		glm::vec3 normal(0,0,0);

		for(int j=0; j<mesh->nfaces; j++) {
			if( (mesh->vertices[faces[j].index[0]][0] == mesh->vertices[i][0] && mesh->vertices[faces[j].index[0]][1] == mesh->vertices[i][1] && mesh->vertices[faces[j].index[0]][2] == mesh->vertices[i][2]) ||
			    (mesh->vertices[faces[j].index[1]][0] == mesh->vertices[i][0] && mesh->vertices[faces[j].index[1]][1] == mesh->vertices[i][1] && mesh->vertices[faces[j].index[1]][2] == mesh->vertices[i][2]) ||
			    (mesh->vertices[faces[j].index[2]][0] == mesh->vertices[i][0] && mesh->vertices[faces[j].index[2]][1] == mesh->vertices[i][1] && mesh->vertices[faces[j].index[2]][2] == mesh->vertices[i][2]) ) {
				   normal += calcFaceNormal(j);
			}
		}

		normal = glm::normalize(normal);

		output << normal.x << " " << normal.y << " " << normal.z << endl;
	}
}

void A3dsHandler::readNormalsFromFile(const char* filename) {
	ifstream input;
	input.open(filename);

	ndata = new float[mesh->nvertices*3];

	for(int i=0; i<mesh->nvertices*3; i++) input >> ndata[i];

	normals = new VBO(ndata, sizeof(float)*mesh->nvertices*3, 1);
}

glm::vec3 A3dsHandler::calcFaceNormal(int i) {
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

		return normal;
}

void A3dsHandler::makeNormalsPerFace() {
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

void A3dsHandler::makeUVs() {
	float *ndata = new float[mesh->nvertices*2];

	for(int i=0; i<mesh->nvertices; i++) {
		ndata[2*i] = mesh->texcos[i][0];
		ndata[2*i+1] = mesh->texcos[i][1];
	}

	UVs = new VBO(ndata, sizeof(float)*mesh->nvertices*2, 2);
}

glm::mat4 A3dsHandler::getModelMatrix() {
	return glm::mat4(mesh->matrix[0][0], mesh->matrix[1][0], mesh->matrix[2][0], mesh->matrix[3][0],
					 mesh->matrix[0][1], mesh->matrix[1][1], mesh->matrix[2][1], mesh->matrix[3][1], 
					 mesh->matrix[0][2], mesh->matrix[1][2], mesh->matrix[2][2], mesh->matrix[3][2], 
					 mesh->matrix[0][3], mesh->matrix[1][3], mesh->matrix[2][3], mesh->matrix[3][3]);
}

void A3dsHandler::makeTBNSpace() {
	float *tdata = new float[mesh->nvertices*3];
	float *bdata = new float[mesh->nvertices*3];

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

		glm::vec2 uv1(mesh->texcos[faces[i].index[0]][0],
					 mesh->texcos[faces[i].index[0]][1]);

		glm::vec2 uv2(mesh->texcos[faces[i].index[1]][0],
					 mesh->texcos[faces[i].index[1]][1]);

		glm::vec2 uv3(mesh->texcos[faces[i].index[2]][0],
					 mesh->texcos[faces[i].index[2]][1]);

		glm::vec3 deltaPos1 = v2-v1;
		glm::vec3 deltaPos2 = v3-v1;


		glm::vec2 deltaUV1 = uv2-uv1;
		glm::vec2 deltaUV2 = uv3-uv1;

		float r = 1.0f / (deltaUV1.x*deltaUV2.y - deltaUV1.y*deltaUV2.x);
		glm::vec3 tangent = (deltaPos1*deltaUV2.y - deltaPos2*deltaUV1.y)*r;
		glm::vec3 bitangent = (deltaPos2*deltaUV1.x - deltaPos1*deltaUV2.x)*r;

		tdata[faces[i].index[0]*3+0] += tangent.x;
		tdata[faces[i].index[0]*3+1] += tangent.y;
		tdata[faces[i].index[0]*3+2] += tangent.z;

		tdata[faces[i].index[1]*3+0] += tangent.x;
		tdata[faces[i].index[1]*3+1] += tangent.y;
		tdata[faces[i].index[1]*3+2] += tangent.z;

		tdata[faces[i].index[2]*3+0] += tangent.x;
		tdata[faces[i].index[2]*3+1] += tangent.y;
		tdata[faces[i].index[2]*3+2] += tangent.z;


		bdata[faces[i].index[0]*3+0] += bitangent.x;
		bdata[faces[i].index[0]*3+1] += bitangent.y;
		bdata[faces[i].index[0]*3+2] += bitangent.z;

		bdata[faces[i].index[1]*3+0] += bitangent.x;
		bdata[faces[i].index[1]*3+1] += bitangent.y;
		bdata[faces[i].index[1]*3+2] += bitangent.z;

		bdata[faces[i].index[2]*3+0] += bitangent.x;
		bdata[faces[i].index[2]*3+1] += bitangent.y;
		bdata[faces[i].index[2]*3+2] += bitangent.z;
	}

	for(int i=0; i<mesh->nvertices; i++) {
		glm::vec3 n(ndata[3*i+0], ndata[3*i+1], ndata[3*i+2]);
		glm::vec3 t(tdata[3*i+0], tdata[3*i+1], tdata[3*i+2]);
		glm::vec3 b(bdata[3*i+0], bdata[3*i+1], bdata[3*i+2]);

		t = glm::normalize(t - n*glm::dot(n,t));

		if(glm::dot(glm::cross(n,t), b) < 0.0f)
			t = t*-1.0f;

		tdata[3*i+0] = t.x;
		tdata[3*i+1] = t.y;
		tdata[3*i+2] = t.z;
	}

	tangents = new VBO(tdata, sizeof(float)*mesh->nvertices*3, 3);
	bitangents = new VBO(bdata, sizeof(float)*mesh->nvertices*3, 4);
}

void A3dsHandler::makeBoundingBox() {
	maximums.x = minimums.x = mesh->vertices[0][0];
	maximums.y = minimums.y = mesh->vertices[0][1];
	maximums.z = minimums.z = mesh->vertices[0][2];

	for(int e=1; e<mesh->nvertices; e++) {
		if(mesh->vertices[e][0] > maximums.x) maximums.x = mesh->vertices[e][0];
		if(mesh->vertices[e][1] > maximums.y) maximums.y = mesh->vertices[e][1];
		if(mesh->vertices[e][2] > maximums.z) maximums.z = mesh->vertices[e][2];

		if(mesh->vertices[e][0] < minimums.x) minimums.x = mesh->vertices[e][0];
		if(mesh->vertices[e][1] < minimums.y) minimums.y = mesh->vertices[e][1];
		if(mesh->vertices[e][2] < minimums.z) minimums.z = mesh->vertices[e][2];
	}

	edges = maximums-minimums;
	center = (maximums+minimums)/2.0f;

	maxDimension = std::max(edges.x, std::max(edges.y, edges.z));
}

#endif