#include "Floor.h"

Floor::Floor(float radius) {
	float quad[] = { 
		-radius, 0.0f, -radius,
		 radius, 0.0f, -radius,
		-radius, 0.0f,  radius,
		 radius, 0.0f,  radius
	};

	GLushort quadI[] = {
		0, 2, 1,
		1, 2, 3
	};

	float quadN[] = { 
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};


	float quadUV[] = { 
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 0.0f, 1.0f,
		 1.0f, 1.0f
	};

	vertexs = new VBO(quad, sizeof(quad), 0);
	normals = new VBO(quadN, sizeof(quadN), 1);
	UVs		= new VBO(quadUV, sizeof(quadUV), 2);
	indexs	= new IBO(quadI, sizeof(quadI));
}