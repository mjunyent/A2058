#include "Storm.h"

Storm::Storm(CSParser *csp) {
	this->csp = csp;

	GLfloat zerozerozero[] = { 0.0, 0.0, 0.0 };

	singlePoint = new VBO(zerozerozero, sizeof(GLfloat)*3, 0);

	myCam = new Camera(csp->data.width, csp->data.height,
					   csp->data.FOV, csp->data.zNear, csp->data.zFar,
					   csp->data.CamPos, csp->data.CamDir, csp->data.CamUp,
					   csp->data.FocalLength, csp->data.FocusDistance, csp->data.FStop);

	myRig = new Rig(*myCam, csp->data.EyeSep);

	bool lecalite[] = { true };
	left = new FBO(csp->data.width, csp->data.height, true, 1, lecalite);
	right = new FBO(csp->data.width, csp->data.height, true, 1, lecalite);

	ballTex = TBO("Images/Balls/ball3T.png", true);
	bgTex   = TBO("Images/Balls/ball2Bg.png", true);

	billboardShad = new Shader("Shaders/Vladivostok/storm.vert", "Shaders/Vladivostok/storm.geom", "Shaders/Vladivostok/storm.frag");
	billboard_M_Id		 = billboardShad->getUniform("Model");
	billboard_V_Id		 = billboardShad->getUniform("View");
	billboard_P_Id		 = billboardShad->getUniform("Projection");
	billboard_up_id		 = billboardShad->getUniform("up");
	billboard_camPos_Id  = billboardShad->getUniform("camPos");
	billboard_r_Id		 = billboardShad->getUniform("r");
	billboard_tex_Id	 = billboardShad->getUniform("tex");
	billboard_texSize_Id = billboardShad->getUniform("texSize");
	billboard_bgTex_Id	 = billboardShad->getUniform("bgTex");
	billboard_depth_Id	 = billboardShad->getUniform("depth");

	M_ball = vector<vec3>(100);
	for(int i=0; i<M_ball.size(); i++) {
		M_ball[i] = vec3(randValue(-125, 125), randValue(-90, 90), randValue(-520, -10));
	}

	dupdate(0);
}

void Storm::draw(int s, double t) {
	sort(M_ball.begin(), M_ball.end(), DepthSort() ); 

	//Mono
/*	currentV = &myCam->V;
	currentCamPos = &myCam->position;
	render(s, t);*/

	//Stereo
	currentV = &myRig->V_left;
	currentCamPos = &myRig->positionL;
	left->bind();
	glClearColor(0.0, 4.0/255.0, 18.0/255.0, 1.0);
	render(s, t);
	left->unbind();

	currentV = &myRig->V_right;
	currentCamPos = &myRig->positionR;
	right->bind();
	glClearColor(0.0, 4.0/255.0, 18.0/255.0, 1.0);
	render(s, t);
	right->unbind();

	outputBuffL = left;
	outputBuffR = right;
}

void Storm::render(int s, double t) {
	billboardShad->use();

	ballTex.bind(0);
	bgTex.bind(1);
	glUniformMatrix4fv(billboard_V_Id, 1, GL_FALSE, &(*currentV)[0][0]);
	glUniformMatrix4fv(billboard_P_Id, 1, GL_FALSE, &myCam->P[0][0]);
	glUniform3fv(billboard_up_id, 1, &myCam->up[0]);
	glUniform3fv(billboard_camPos_Id, 1, &(*currentCamPos)[0]);
	glUniform1f(billboard_r_Id, quadSize);
	glUniform1i(billboard_tex_Id, 0);
	glUniform1f(billboard_texSize_Id, texSize);
	glUniform1i(billboard_bgTex_Id, 1);

	for(int i=0; i<M_ball.size(); i++) {
		mat4 idd = translate(M_ball[i]);
		glUniformMatrix4fv(billboard_M_Id, 1, GL_FALSE, &idd[0][0]); 
		glUniform1f(billboard_depth_Id, fabs(M_ball[i].z)/500.0f);
		singlePoint->enable(3);
		singlePoint->draw(GL_POINTS);
		singlePoint->disable();
	}
}

void Storm::dupdate(double t) {
	if(csp->getf("Spheres.RenderBox") >= 1.0) {
		billboardShad = new Shader("Shaders/Vladivostok/storm.vert", "Shaders/Vladivostok/storm.geom", "Shaders/Vladivostok/storm.frag");
		billboard_M_Id		 = billboardShad->getUniform("Model");
		billboard_V_Id		 = billboardShad->getUniform("View");
		billboard_P_Id		 = billboardShad->getUniform("Projection");
		billboard_up_id		 = billboardShad->getUniform("up");
		billboard_camPos_Id  = billboardShad->getUniform("camPos");
		billboard_r_Id		 = billboardShad->getUniform("r");
		billboard_tex_Id	 = billboardShad->getUniform("tex");
		billboard_texSize_Id = billboardShad->getUniform("texSize");
		billboard_depth_Id	 = billboardShad->getUniform("depth");
	}

	csp->parse();
	csp->passToCam(myCam);
	csp->passToRig(myRig);

	float v = csp->getf("Spheres.Velocity");
	float zMax = csp->getf("Spheres.zMax");
	float xMargin = csp->getf("Spheres.xMargin");
	float yMargin = csp->getf("Spheres.yMargin");
	float zSpawnMin = csp->getf("Spheres.zSpawnMin");
	float zSpawnMax = csp->getf("Spheres.zSpawnMax");

	for(int i=0; i<M_ball.size(); i++) {
		M_ball[i].z += v;
		if(M_ball[i].z > zMax) M_ball[i] = vec3(randValue(-xMargin, xMargin), randValue(-yMargin, yMargin), randValue(zSpawnMin, zSpawnMax));
	}

	quadSize = csp->getf("Spheres.Size");
	texSize = csp->getf("Spheres.texSize");

}
void Storm::update(double t) {
	for(int i=0; i<M_ball.size(); i++) {
		M_ball[i].z += 1.5;
		if(M_ball[i].z > 100) M_ball[i] = vec3(randValue(-125, 125), randValue(-80, 80), randValue(-520, 500));
	}
}