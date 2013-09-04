#include "FloatingBalls.h"

FloatingBalls::FloatingBalls(CSParser *csp) : Deferred() {
	this->csp = csp;
	csp->parse();

	myCam = new Camera(csp->data.width, csp->data.height,
					   csp->data.FOV, csp->data.zNear, csp->data.zFar,
					   csp->data.CamPos, csp->data.CamDir, csp->data.CamUp,
					   csp->data.FocalLength, csp->data.FocusDistance, csp->data.FStop);

	myRig = new Rig(*myCam, csp->data.EyeSep);

	secondShad = new Shader("Shaders/Deferred/second.vert", "Shaders/Vladivostok/floating.frag");
	ballGradientID = secondShad->getUniform("gradient");

	setup(myRig);

	ballGradient = TBO("Images/Balls/gradient1.fw.png", true);

	M_world = glm::translate(0,0,0);

	box = new SkyBox(100.0);
	box_model = new Model(firstShad,
						  box->vertexs,
						  box->normals,
						  NULL,
						  NULL,
						  NULL,
						  box->indexs,
						  0.4,
						  glm::vec3(0,0,0),
						  glm::vec3(0,0,0),
						  0.512f,
						  &M_world,
						  1.0,
						  NULL,
						  NULL);

	sphere = new Sphere(2.0, 30);
	M_sphere = glm::translate(0.0f, 9.0f, -20.0f);
	sphere_model = new Model(firstShad,
							 sphere->vertexs,
							 sphere->normals,
							 NULL,
							 NULL,
							 NULL,
							 sphere->indexs,
							 0.4,
							 glm::vec3(1, 1, 1),
							 glm::vec3(1, 1, 1),
							 0.512f,
							 &M_sphere,
							 1.0,
							 NULL,
							 NULL);


	//lights->addSpotLight(glm::vec3(0.0, 20.0, 0.0), glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.80, 0.96, 2.0);
//	lights->addDirectionalLight(glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, 1.0, -1.0), glm::vec3(1.0, 1.0, 1.0));
	lights->addPointLight(glm::vec3(0.0, 0.0, 0.0), glm::vec3(2.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
//	lights->addDirectionalLight(glm::vec3(2.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	
//	dotheAO(30, 0.05, glm::vec2(1, 5));
	dotheDOF(true);

	M_ball = vector<vec3>(250);
	for(int i=0; i<M_ball.size(); i++) {
		M_ball[i] = vec3(randValue(-125, 125), randValue(-90, 90), randValue(-120, -10));
	}
}

void FloatingBalls::SecondPass() {
	if(doAO || doDOF || doAA || doOffscreen || doStereo) SecondRenderBuff->bind();
	glm::mat4 invPV = glm::inverse(cam->P * *currentV);  //Maybe do this in update.
	glDisable(GL_DEPTH_TEST);

	secondShad->use();
	currentRenderBuffer->bind_texture(0, 0);
	currentRenderBuffer->bind_texture(1, 1);
	currentRenderBuffer->bind_texture(2, 2);
	currentRenderBuffer->bind_depth_texture(3);
	
	//ADDED
	ballGradient.bind(5);
	glUniform1i(ballGradientID, 5);

	lights->passLightToGPU();

	if(backgroundID != -1) {
		backgroundTex.bind(4);
		glUniform1i(isBgTexID, 1);
		glUniform1i(backgroundID, 4);
	} else {
		glUniform1i(isBgTexID, 0);
		glUniform3fv(bgColorID, 1, &(*bg_color)[0]); 
	}

	glUniform1i(normalID, 0);
	glUniform1i(diffuseID, 1);
	glUniform1i(specularID, 2);
	glUniform1i(depthID, 3);

	glUniformMatrix4fv(invPVID, 1, GL_FALSE, &invPV[0][0]);
	glUniform3fv(camPosID, 1, &(*currentCamPos)[0]);

	screen_quad->enable(3);
	screen_quad_I->draw(GL_TRIANGLES);
	screen_quad->disable();

	glEnable(GL_DEPTH_TEST);
	if(doAO || doDOF || doAA || doOffscreen || doStereo) SecondRenderBuff->unbind();
}

void FloatingBalls::render(int s, double t) {
	if(renderBox) box_model->render();
	
	for(int i=0; i<M_ball.size(); i++) {
		M_sphere = translate(M_ball[i]);
		sphere_model->render();
	}
}

void FloatingBalls::update(double t) {
	csp->parse();
	csp->passToLight(this->lights);

	float v = csp->getf("Spheres.Velocity");
	float zMax = csp->getf("Spheres.zMax");

	for(int i=0; i<M_ball.size(); i++) {
		M_ball[i].z += v;
		if(M_ball[i].z > zMax) M_ball[i] = vec3(randValue(-125, 125), randValue(-90, 90), randValue(-120, -10));
	}

	if(csp->getf("Spheres.RenderBox") >= 1.0) renderBox = true;
	else renderBox = false;

//	M_world *= glm::rotate(0.1f, glm::vec3(0.0f, 1.0f, 0.0f));

	cam->fov			= csp->data.FOV;
	cam->znear			= csp->data.zNear;
	cam->zfar			= csp->data.zFar;
	cam->position		= csp->data.CamPos;
	cam->direction		= csp->data.CamDir;
	cam->up				= csp->data.CamUp;
	cam->focalLength	= csp->data.FocalLength;
	cam->focusDistance	= csp->data.FocusDistance;
	cam->FStop			= csp->data.FStop;

	rig->eye_sep		= csp->data.EyeSep;

	M_sphere = translate(csp->toVec3(csp->pr.get<std::string>("TestSphere.Position")));

	rig->update(t);
}