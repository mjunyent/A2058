#include "SkyBox.h"

SkyBox::SkyBox(float radius) {
	float cube[] = {
		//Top
		-radius,  radius, -radius, //0
		 radius,  radius, -radius, //1
		 radius,  radius,  radius, //2
		-radius,  radius,  radius, //3

		//Bottom
		-radius, -radius, -radius, //4
		 radius, -radius, -radius, //5
		 radius, -radius,  radius, //6
		-radius, -radius,  radius, //7

		//Front
		-radius,  radius,  radius, //8   //3
		 radius,  radius,  radius, //9   //2
	  	 radius, -radius,  radius, //10  //6
		-radius, -radius,  radius, //11  //7

		//Back
		-radius,  radius, -radius, //12  //0
		 radius,  radius, -radius, //13  //1
		 radius, -radius, -radius, //14  //5
 		-radius, -radius, -radius, //15  //4

		//Right
		 radius,  radius,  radius, //16  //2
		 radius,  radius, -radius, //17  //1
 		 radius, -radius, -radius, //18  //5
		 radius, -radius,  radius, //19  //6

		 //Left
 		-radius,  radius,  radius, //20  //3
		-radius,  radius, -radius, //21  //0
		-radius, -radius, -radius, //22  //4
		-radius, -radius,  radius, //23  //7
	};

	GLushort cubeI[] = {
		//Top
		0, 1, 3,
		1, 2, 3,

		//Bottom
		4, 7, 5,
		5, 7, 6,

		//Front
		8, 9, 11,
		9, 10, 11,

		//Back
		12, 15, 13,
		13, 15, 14,

		//Right
		16, 17, 19,
		17, 18, 19,

		//Left
		20, 23, 21,
		21, 23, 22
	};

	float cubeN[] = {
		//Top
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,

		//Bottom
		 0.0f, 1.0f,  0.0f,
		 0.0f, 1.0f,  0.0f,
		 0.0f, 1.0f,  0.0f,
		 0.0f, 1.0f,  0.0f,

 		//Front
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,

 		//Back
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,

 		//Right
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,

		//Right
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f
	};

	vertexs = new VBO(cube, sizeof(cube), 0);
	normals = new VBO(cubeN, sizeof(cubeN), 1);
//	UVs		= new VBO(quadUV, sizeof(quadUV), 2);
	indexs	= new IBO(cubeI, sizeof(cubeI));
}