#include "VesselScene.h"

VesselScene::VesselScene(Shader *shader, GLFWmutex mutex, int *status, int *command) {
	this->mutex = mutex;
	this->status = status;
	this->command = command;

	vessel = new A3dsHandler("Models/vessel.3ds", 1);
	vessel->calculateNormals();

	cell = new A3dsHandler("Models/cell2.3ds", 0);
	cell->calculateNormals();

//	crossSection = new A3dsHandler("Models/cellCrossSection.3ds");

	P = glm::perspective(75.0f, 16.0f/9.0f, 0.1f, 20.0f);

	M_Vessel = glm::rotate(90.0f, glm::vec3(1, 0, 0));

	for(int i=0; i<40; i++) {
//		Erythrocyte t(0.5, 0.7*(float(rand()%20000)/10000.0-1.0), float(rand()%20000)/10000.0-0.5);
		float dist = 10E6;
		int count = 0;
		glm::vec3 p;
		do {
			p.x = randValue(0.0, 1.0);
			p.y = randValue(0.1, 0.55);
			p.z = randValue(0.0, 2*3.1415);
			Erythrocyte xx(p.x, p.y*sin(p.z), p.y*cos(p.z));

			for(int j=0; j<i; j++) {
				float ddist = glm::length(xx.position-globuline[j].position);
				if(ddist < dist) dist = ddist;
			}
			count++;
		} while(dist < 0.036 && i >= 1 && count < 1000);
		if(count >= 1000)	break;

		Erythrocyte t(p.x, p.y*sin(p.z), p.y*cos(p.z));

		globuline.push_back(t);
	}

	cout << "Number of erythrocytes: " << globuline.size() << endl;

	Global_acc = 0.0;
	Global_vel = 0.0;
	Beat = false;
	nearestRBC = 0;

	appear = 0;
	appearing = false;
	disappearing = false;
	skipNoInfo = false;

	vessel_model = new Model(shader,
							 vessel->vertexs,
							 vessel->normals,
							 vessel->indexs,
							 0.2,
							 glm::vec3(156.0/255.0f, 9.0/255.0f, 9.0/255.0f),
							 glm::vec3(0.4f, 0.4f, 0.4f),
							 0.512f,
							 &M_Vessel,
							 1.0f);

	cell_model = new Model(shader,
						   cell->vertexs,
						   cell->normals,
						   cell->indexs,
						   0.2,
						   glm::vec3(214.0f/255.0f, 34.0/255.0f, 6.0/255.0f),
						   glm::vec3(1.0f, 1.0f, 1.0f),
						   0.256f,
						   NULL,
						   0.15f);

	this->shader = shader;

	M_Id = shader->getUniform("Model");
	V_Id = shader->getUniform("View");
	P_Id = shader->getUniform("Projection");
	Scale_Id = shader->getUniform("scale");

}

void VesselScene::update(double t) {
	glfwLockMutex(mutex);
	if(*status == 0) {
		if(*command == 0) {
			if(global::song->playVel < 1.75) global::song->setVel(global::song->playVel+0.05);
			*command = -1;
			cout << "Velocity: " << global::song->playVel << endl;
		} else if(*command == 1) {
			if(global::song->playVel > 0.85) global::song->setVel(global::song->playVel-0.05);
			*command = -1;
			cout << "Velocity: " << global::song->playVel << endl;
		} else if(*command == 2) {
			global::song->Stop();
			*status = 1;
			*command = -1;
		}
	} else if(*status == 1) {
		if(*command == 0) {
			*status = 0;
			*command = -1;
			global::song->Play();
		} else if(*command == 1) {
			*status = 2;
			appear = 0.0;
			appearing = true;
			*command = -1;
		}
	} else if(*status == 2) {
		if(*command == 0) {
			appearing = false;
			disappearing = true;
			skipNoInfo = true;
			appear = 0.98;
			*command = -1;
		} else if(*command == 1) {
			appearing = false;
			disappearing = true;
			skipNoInfo = false;
			appear = 0.98;
			*command = -1;
		}
	}
	glfwUnlockMutex(mutex);

	if(*status == 0) updatePlay();
	if(*status == 1) updateStop();
	if(*status == 2) updateInfo();

}

void VesselScene::updatePlay() {
	if(global::song->SoundTime() > 0.40) Beat = false;
	else if(global::song->SoundTime() > 0.1) Beat = true;

	if(Beat) {
		Global_acc = 0.0015*global::song->playVel;
		VelSet = false;
	} else {
		if(Global_vel > (global::song->playVel-1.0)/50.0 && !VelSet) {
			Global_acc = -0.0008*global::song->playVel;
		} else {
			VelSet = true;
			Global_acc = 0;
		}
	}

	Global_vel += Global_acc;

	for(int i=0; i<globuline.size(); i++) {
		globuline[i].a = Global_acc;
		globuline[i].v = Global_vel;
		globuline[i].update();

		if(globuline[i].p > 1.0) {
			float dist = 10E6;
			int count = 0;
			glm::vec3 p;
			do {
				p.x = randValue(-0.4, 0.2);
				p.y = randValue(0.1, 0.55);
				p.z = randValue(0.0, 2*3.1415);
				Erythrocyte xx(p.x, p.y*sin(p.z), p.y*cos(p.z));

				for(int j=0; j<globuline.size(); j++) {
					float ddist = glm::length(xx.position-globuline[j].position);
					if(ddist < dist) dist = ddist;
				}
				count++;
			} while(dist < 0.04 && count < 1000);
			if(count < 1000) {
				globuline[i].p = p.x;
				globuline[i].pY = p.y*sin(p.z);
				globuline[i].pZ = p.y*cos(p.z);
				globuline[i].a = Global_acc;
				globuline[i].v = Global_vel;
			}
		}
	}
}

void VesselScene::updateStop() {
	Global_vel = 0.0;
	Global_acc = 0.0;

	for(int i=0; i<globuline.size(); i++) {
		globuline[i].a = Global_acc;
		globuline[i].v = Global_vel;

		if(globuline[i].p < 1.1 && globuline[i].p > 0.82) { //the erythrocites too near keep advancing...
			globuline[i].v = 0.005;
		}
		globuline[i].update();
	}

	getNearest();
}

void VesselScene::updateInfo() {
	updateStop();

	if(appearing) appear += 0.02;
	if(disappearing) appear -= 0.05;

	if(appear >= 0.98) {
		appear = 0.98;
		appearing = false;
	}
	if(appear < 0.0) {
		appear = 0.0;
		disappearing = false;
		glfwLockMutex(mutex);
			if(skipNoInfo) { 
				*status = 0;
				global::song->Play();
			} else *status = 1;
		glfwUnlockMutex(mutex);
	}
}

void VesselScene::getNearest() {
	int nearestId = 0;
	float nearestP = -1;

	for(int i=0; i<globuline.size(); i++) {
		if(globuline[i].p >= nearestP && globuline[i].p <= 0.82) {
			nearestId = i;
			nearestP = globuline[i].p;
		}
	}

	nearestRBC = nearestId;
}


void VesselScene::renderiseee(glm::mat4 &VA) {
	glUniformMatrix4fv(V_Id, 1, GL_FALSE, &VA[0][0]);
	glUniformMatrix4fv(P_Id, 1, GL_FALSE, &P[0][0]);

	vessel_model->render();

	invPV = glm::inverse(P*VA);

	for(int i=0; i<globuline.size(); i++) {
		cell_model->M = &(globuline[i].M);
		cell_model->render();
	}
}

glm::vec3 VesselScene::bezier(double t) {
	glm::vec2 p1(-3, 0);
	glm::vec2 p2(-6, 7.2);
	glm::vec2 p3(1, 2);
	glm::vec2 p4(3, 7.2);

	glm::vec2 r;

	p1 *= pow(1-t, 3);
	p2 *= 3*pow(1-t, 2)*t;
	p3 *= 3*(1-t)*t*t;
	p4 *= pow(t, 3);

	r = p1 + p2 + p3 + p4;

	return vec3(r.x, r.y, 0.0);
}

Erythrocyte::Erythrocyte(float start_position, float p_Z, float p_Y) {
	p = start_position;
	pY = p_Y;
	pZ = p_Z;

	v = 0.0;
	angle = 0;
	angv = randValue(0.1,1.0);

	rotateV = glm::vec3(randValue(0,1), randValue(0,1), randValue(0,1));

	update();
}

void Erythrocyte::update() {
//Update values
	p += v;
//	v += a;
	angle += v*100 + angv;//*angv;

//Calculate new matrix
	position = bezier(p);
//	position.z = 1.1;
	M = glm::rotate(90.0f, glm::vec3(1, 0, 0))								//90 degrees rotation to align with vessel.
		*glm::translate(position)											//translate according to the bezier's spline
		*glm::rotate(glm::degrees(acos(dbezier(p).x/glm::length(dbezier(p)))), glm::vec3(0,0,1)) //rotation to be perpendicular to bezier's tangent, (now Y,Z are on the bezier's perpendicular plane, X is to advance)
		*glm::translate(glm::vec3(0.0, pY, pZ+0.8))							//translate to move around the center of the spline
		*glm::rotate(angle, rotateV);										//rotation of the erythrocyte
}

glm::vec3 Erythrocyte::bezier(double t) {
	glm::vec2 p1(-3, 0);
	glm::vec2 p2(-6, 7.2);
	glm::vec2 p3(1, 2);
	glm::vec2 p4(3, 7.2);

	glm::vec2 r;

	p1 *= pow(1-t, 3);
	p2 *= 3*pow(1-t, 2)*t;
	p3 *= 3*(1-t)*t*t;
	p4 *= pow(t, 3);

	r = p1 + p2 + p3 + p4;

	return vec3(r.x, r.y, 0.0);
}

glm::vec2 Erythrocyte::dbezier(double t) {
	glm::vec2 p1(-3, 0);
	glm::vec2 p2(-6, 7.2);
	glm::vec2 p3(1, 2);
	glm::vec2 p4(3, 7.2);

	glm::vec2 r;

	p1 *= -3*(t-1)*(t-1);
	p2 *= 9*t*t - 12*t + 3;
	p3 *= 3*(2-3*t)*t;
	p4 *= 3*t*t;

	r = p1 + p2 + p3 + p4;

	return r;
}



VesselRender::VesselRender(VesselScene* Vs, glm::mat4 *V) {
	this->Vs = Vs;
	this->V = V;
}

void VesselRender::draw(double t) {
	invPV = glm::inverse(Vs->P * *V);
	Vs->renderiseee(*V);
}



InfoRender::InfoRender(VesselScene* Vs, glm::mat4 *V, FBO *background, FBO *target) {
	this->Vs = Vs;
	this->V = V;
	this->background = background;
	this->renderTarget = target;

	image = new TBO("Images/RBCInfo.png", true);

	Box = new VBO(global::quad, sizeof(global::quad), 0);
	BoxI = new IBO(global::quad_I, sizeof(global::quad_I));

	third = new Shader("Shaders/InfoRender.vert", "Shaders/Inforender.frag");
	bg_Id = third->getUniform("background");
	im_Id = third->getUniform("image");

	topId = third->getUniform("top");
	downId = third->getUniform("down");
	leftId = third->getUniform("left");
	rightId = third->getUniform("right");
	drawId = third->getUniform("draw");
	appearId = third->getUniform("appear");
}


void InfoRender::draw(double t) {
	glDisable(GL_DEPTH_TEST);

	renderTarget->bind();

	third->use();
		background->bind_texture(0, 0);
		image->bind(1);
		glUniform1i(bg_Id, 0);
		glUniform1i(im_Id, 1);

		if(*(Vs->status) == 2) glUniform1i(drawId, 1);
		else glUniform1i(drawId, 0);
		
		if(*(Vs->status) == 2) {
			glm::vec4 p = Vs->P  * (*V) * Vs->globuline[Vs->nearestRBC].M * glm::vec4(0,0,0,1);

			glm::vec2 np(p.x/p.w, p.y/p.w);

			np = screen2normalized(np);

			glUniform1f(topId, np.y);
			glUniform1f(downId, np.y-0.4);

			glUniform1f(leftId, np.x-0.4);
			glUniform1f(rightId, np.x);

			glUniform1f(appearId, Vs->appear);
		}
		Box->enable(3);
		BoxI->draw(GL_TRIANGLES);
		Box->disable();

	renderTarget->unbind();

	glEnable(GL_DEPTH_TEST);
}
