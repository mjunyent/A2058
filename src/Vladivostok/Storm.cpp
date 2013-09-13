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

	ballTex = TBO("Images/Balls/cell3G.png", true);
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

	c = new Cells(40, 
		csp->getf("Cells.Velocity"),
		csp->getf("Cells.xRange"),
		csp->getf("Cells.yRange"),
		csp->getf("Cells.zNear"),
		csp->getf("Cells.zFar"),
		csp->getf("Cells.zFarAway"),
		csp->getf("Cells.K"),
		csp->getf("Cells.L"),
		csp->getf("Cells.M"));

	s = new Scanner(vec3(csp->getf("Scan.box.left"), csp->getf("Scan.box.up"), csp->getf("Scan.box.near")),
					vec3(csp->getf("Scan.box.right"), csp->getf("Scan.box.down"), csp->getf("Scan.box.far")),
					c);
	s->debSetup();
}

void Storm::draw(int s, double t) {
	//Mono
/*	currentV = &myCam->V;
	currentCamPos = &myCam->position;
	render(s, t);*/
	mat4 idd = translate(0.0f, 0.0f, 0.0f);

	//Stereo
	currentV = &myRig->V_left;
	currentCamPos = &myRig->positionL;
	left->bind();
//	glClearColor(0.0, 4.0/255.0, 18.0/255.0, 1.0);
	render(s, t);
	this->s->renderDebugBox(&idd, currentV, &myCam->P);
	left->unbind();

	currentV = &myRig->V_right;
	currentCamPos = &myRig->positionR;
	right->bind();
//	glClearColor(0.0, 4.0/255.0, 18.0/255.0, 1.0);
	render(s, t);
	this->s->renderDebugBox(&idd, currentV, &myCam->P);
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

	for(int i=0; i<c->cells.size(); i++) {
		mat4 idd = translate(c->cells[i].p);
		glUniformMatrix4fv(billboard_M_Id, 1, GL_FALSE, &idd[0][0]); 
		glUniform1f(billboard_depth_Id, fabs(c->cells[i].p.z/c->zFar));
		singlePoint->enable(3);
		singlePoint->draw(GL_POINTS);
		singlePoint->disable();
	}
}

void Storm::update(double t) {
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

	quadSize = csp->getf("Spheres.Size");
	texSize = csp->getf("Spheres.texSize");

	
	c->vel = csp->getf("Cells.Velocity");
	c->xRange =	csp->getf("Cells.xRange");
	c->yRange =	csp->getf("Cells.yRange");
	c->zNear = csp->getf("Cells.zNear");
	c->zFar = csp->getf("Cells.zFar");
	c->zFarAway = csp->getf("Cells.zFarAway");
	c->K = csp->getf("Cells.K");
	c->L = csp->getf("Cells.L");
	c->M = csp->getf("Cells.M");
	
	s->upLeftNear = vec3(csp->getf("Scan.box.left"), csp->getf("Scan.box.up"), csp->getf("Scan.box.near"));
	s->downRightFar = vec3(csp->getf("Scan.box.right"), csp->getf("Scan.box.down"), csp->getf("Scan.box.far"));

	c->update();
	s->detect();
}

Cells::Cells(int n, float v, float xRange, float yRange, float zNear, float zFar, float zFarAway, float K, float L, float M) {
	cells = vector<Cell>(n);

	move = true;
	vel = v;
	this->xRange	= xRange;
	this->yRange	= yRange;
	this->zNear		= zNear;
	this->zFar		= zFar;
	this->zFarAway	= zFarAway;
	this->K			= K;
	this->L			= L;
	this->M			= M;

	for(int i=0; i<n; i++) {
		cells[i].p = vec3(randValue(-xRange, xRange),
						  randValue(-yRange, yRange),
						  randValue(std::min(zNear, zFarAway), std::max(zNear, zFarAway)));
		cells[i].v = vec3(0, 0, vel);
	}

	deflector = vec3(0.0, 0.0, 0.0);
}

void Cells::update() {
	if(!move) return;

	for(int i=0; i<cells.size(); i++) {
		cells[i].v -= K*(vec3(0,0,vel) - cells[i].v);

		if(cells[i].p.z > zNear) {
			cells[i].p = vec3(randValue(-xRange, xRange),
						  randValue(-yRange, yRange),
						  randValue(std::min(zFar, zFarAway), std::max(zFar, zFarAway)));

			cells[i].v = vec3(0, 0, vel);
		}

		for(int j=0; j<cells.size(); j++) {
			if(j != i) {
				vec2 p, pp;
				p  = vec2(cells[i].p.x, cells[i].p.y);
				pp = vec2(cells[j].p.x, cells[j].p.y);
				float l = length(p - pp);
				if(l <= L) {
					p = M/l * (p-pp)/l;
					cells[i].v.x += p.x;
					cells[i].v.y += p.y;
				}
			}
		}

		vec2 p, pp;
		p  = vec2(cells[i].p.x, cells[i].p.y);
		pp = vec2(deflector.x, deflector.y);
		float l = length(cells[i].p - deflector);
		if(l <= L) {
			p = M/l * (p-pp)/l;
			cells[i].v.x += p.x;
			cells[i].v.y += p.y;
		}

		cells[i].p += cells[i].v;	
	}

	sort(cells.begin(), cells.end(), DepthSort() ); 
}

Scanner::Scanner(vec3 upLeftNear, vec3 downRightFar, Cells *cells) {
	this->upLeftNear = upLeftNear;
	this->downRightFar = downRightFar;
	this->cells = cells;
	TRESHOLD_STOP = 2;
	TRESHOLD_NEXT = 2;
	LAST = 0;
}

void Scanner::detect() {
	if(!cells->move) { //If we are on STOP
		if(director::currentTime-LAST > TRESHOLD_STOP) {
			cells->move = true; //HIT PLAY!
			LAST = director::currentTime;
		}
		return;
	}

	vector<vec3> p;

	for(int i=0; i<cells->cells.size(); i++) {
		if(inRange(cells->cells[i].p.x, upLeftNear.x, downRightFar.x) &&
		   inRange(cells->cells[i].p.y, upLeftNear.y, downRightFar.y) &&
		   inRange(cells->cells[i].p.z, upLeftNear.z, downRightFar.z)) {
			   p.push_back(cells->cells[i].p);
		}
	}

	if(p.size() >= 1) {
		TOBAGO::log.write(DEBUG) << p.size() << " cells Detected";
		if(director::currentTime-LAST > TRESHOLD_NEXT) {
			cells->move = false;
			LAST = director::currentTime;
		}
	}
}

void Scanner::draw() {



}

void Scanner::debSetup() {
	debShad = new Shader("Shaders/debugLines.vert", "Shaders/debugLines.frag");
	deb_M_Id		 = debShad->getUniform("Model");
	deb_V_Id		 = debShad->getUniform("View");
	deb_P_Id		 = debShad->getUniform("Projection");
	deb_Color_Id	 = debShad->getUniform("Color");
	debBox = NULL;
}

void Scanner::renderDebugBox(glm::mat4 *M, glm::mat4 *V, glm::mat4 *P) {
	float left = upLeftNear.y;
	float right = downRightFar.y;
	float up = upLeftNear.x;
	float down = downRightFar.x;
	float near = upLeftNear.z;
	float far = downRightFar.z;

	float quadFront[] = { 
		up, left, near,	//0 UP, LEFT
		up, right, near, //1 UP, RIGHT

		up, right, near, //1 UP, RIGHT
		down, right, near, //2 DOWN, RIGHT

		down, right, near, //2 DOWN, RIGHT
		down, left, near,  //3 DOWN, LEFT

		down, left, near,  //3 DOWN, LEFT
		up, left, near,	//0 UP, LEFT

		up, left, far,	//0 UP, LEFT
		up, right, far, //1 UP, RIGHT

		up, right, far, //1 UP, RIGHT
		down, right, far, //2 DOWN, RIGHT

		down, right, far, //2 DOWN, RIGHT
		down, left, far,  //3 DOWN, LEFT

		down, left, far,  //3 DOWN, LEFT
		up, left, far	//0 UP, LEFT
	};

	if(debBox != NULL) debBox->destroy();

	debBox = new VBO(quadFront, sizeof(quadFront), 0);

	debShad->use();
	glUniformMatrix4fv(deb_M_Id, 1, GL_FALSE, &(*M)[0][0]);
	glUniformMatrix4fv(deb_V_Id, 1, GL_FALSE, &(*V)[0][0]);
	glUniformMatrix4fv(deb_P_Id, 1, GL_FALSE, &(*P)[0][0]);
	glUniform3f(deb_Color_Id, 1.0, 0.0, 0.0);

	debBox->enable(3);
	debBox->draw(GL_LINES);
	debBox->disable();
}