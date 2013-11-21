#include "Sphere.h"

Sphere::Sphere(float radius, int sections) {
	int nVertFace = (1+sections)*(1+sections);

	// 6 faces.
	vector<vec3> vertexs(6*nVertFace);
	vector<GLushort> indices;

	for(int i=0; i<=sections; i++) {
		for(int j=0; j<=sections; j++) {
			vertexs[0*nVertFace + i + (sections+1)*j] = vec3(1.0-2.0*(float(i)/sections),
									  1.0-2.0*(float(j)/sections),
									  1.0);
			vertexs[1*nVertFace + i + (sections+1)*j] = vec3(1.0-2.0*(float(i)/sections),
									  1.0-2.0*(float(j)/sections),
									  -1.0);
			vertexs[2*nVertFace + i + (sections+1)*j] = vec3(1.0-2.0*(float(i)/sections),
									  1.0,
									  1.0-2.0*(float(j)/sections));
			vertexs[3*nVertFace + i + (sections+1)*j] = vec3(1.0-2.0*(float(i)/sections),
									  -1.0,
									  1.0-2.0*(float(j)/sections));
			vertexs[4*nVertFace + i + (sections+1)*j] = vec3(1.0,
									  1.0-2.0*(float(i)/sections),
									  1.0-2.0*(float(j)/sections));
			vertexs[5*nVertFace + i + (sections+1)*j] = vec3(-1.0,
									  1.0-2.0*(float(i)/sections),
									  1.0-2.0*(float(j)/sections));
		}
	}

	for(int i=0; i<sections; i++) {
		for(int j=0; j<sections; j++) {
			for(int k=0; k<6; k++) {
				indices.push_back( k*nVertFace + i + (sections+1)*j );
				indices.push_back( k*nVertFace + i+1 + (sections+1)*j );
				indices.push_back( k*nVertFace + i + (sections+1)*(j+1) );

				indices.push_back( k*nVertFace + i + (sections+1)*(j+1) );
				indices.push_back( k*nVertFace + i+1 + (sections+1)*j );
				indices.push_back( k*nVertFace + i+1 + (sections+1)*(j+1) );
			}
		}
	}

	float *vdata = new float[3*vertexs.size()];
	float *ndata = new float[3*vertexs.size()];

	for(int i=0; i<vertexs.size(); i++) {
		vec3 tmp = normalize(vertexs[i]);

		ndata[3*i+0] = tmp.x;
		ndata[3*i+1] = tmp.y;
		ndata[3*i+2] = tmp.z;

		vdata[3*i+0] = tmp.x*radius;
		vdata[3*i+1] = tmp.y*radius;
		vdata[3*i+2] = tmp.z*radius;
	}

	this->vertexs = new VBO(vdata, sizeof(float)*vertexs.size()*3, 0);
	this->normals = new VBO(ndata, sizeof(float)*vertexs.size()*3, 1);

	this->indexs = new IBO(indices);
}