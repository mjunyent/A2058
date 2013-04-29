#include "VesselScene.h"

VesselScene::VesselScene(Shader *shader, glm::mat4 *V) {
	vessel = new A3dsHandler("Models/vessel.3ds", 1);
	vessel->calculateNormals();

	cell = new A3dsHandler("Models/cell2.3ds", 0);
	cell->calculateNormals();

	P = glm::perspective(75.0f, 16.0f/9.0f, 0.1f, 20.0f);

	this->rig = NULL;
	this->V = V;
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
		} while(dist < 0.034 && i >= 1 && count < 1000);
		if(count >= 1000) { 
			cout << "PIPE BROKE, no more Erythrocytes fit!" << endl;
			break;
		}

		Erythrocyte t(p.x, p.y*sin(p.z), p.y*cos(p.z));

		globuline.push_back(t);
	}

	cout << "Number of erythrocytes: " << globuline.size() << endl;

	Global_acc = 0.0;
	Global_vel = 0.0;
	Beat = false;

	beatThresholdVolume = 0.03f;	// The threshold over which to recognize a beat
	beatPostIgnore = 250;			// Number of ms to ignore track for after a beat is recognized
	beatLastTick = 0;				// Time when last beat occurred


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

VesselScene::VesselScene(Shader *shader, Rig *rig) {
	vessel = new A3dsHandler("Models/vessel.3ds", 1);
	vessel->calculateNormals();

	cell = new A3dsHandler("Models/cell.3ds", 0);
	cell->calculateNormals();

	P = glm::perspective(75.0f, 4.0f/3.0f, 0.1f, 100.0f);
	this->rig = rig;
	this->V = NULL;
	M_Vessel = glm::rotate(90.0f, glm::vec3(1, 0, 0));

	for(int i=0; i<12; i++) {
		Erythrocyte t(double(i)/22.0, 0, 0);
		globuline.push_back(t);
	}

	this->shader = shader;

	M_Id = shader->getUniform("Model");
	V_Id = shader->getUniform("View");
	P_Id = shader->getUniform("Projection");
	Scale_Id = shader->getUniform("scale");
}

void VesselScene::draw(double t) {
	if(V != NULL) renderiseee(*V);
	else if(rig != NULL) {
		rig->right->bind();
		renderiseee(rig->V_Right);
		rig->right->unbind();

		rig->left->bind();
		renderiseee(rig->V_Left);
		rig->left->unbind();
	}
}

void VesselScene::update(double t) {
	global::song->getSpectrum();

	bool beatDetected = false;
	if(maxBeat(0, 10) >= beatThresholdVolume && beatLastTick == 0) {
	  beatLastTick = GetTickCount();
	  beatDetected = true;
	}
 
	if (beatDetected) {
	//	Beat = !Beat;
//		if(Beat) cout << "PU: " << global::song->SoundTime() << endl;
//		else cout << "PUM: " << global::song->SoundTime() << endl;
	}
 
	// If the ignore time has expired, allow testing for a beat again
	if (GetTickCount() - beatLastTick >= beatPostIgnore)
	  beatLastTick = 0;

	if(global::song->SoundTime() > 0.40) Beat = false;
	else if(global::song->SoundTime() > 0.1) Beat = true;
//	if(Beat && GetTickCount() - beatLastTick >= 600) Beat = false; 

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
/*		if(Beat) {
			globuline[i].a = 0.0012;
			VelSet = false;
		} else {
			if(globuline[i].v > -0.005 && !VelSet) {
				globuline[i].a = -0.0015;
			} else {
				VelSet = true;
				globuline[i].a = 0;
				//globuline[i].v = 0.002;
			}
		}*/

		globuline[i].a = Global_acc;
		globuline[i].v = Global_vel;

		//globuline[i].v = global::song->FFT[2]/10.0;
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
			} while(dist < 0.03 && i >= 1 && count < 1000);
			if(count >= 1000) { 
		//		cout << "PIPE BROKE, not enought space to update erythrocytes" << endl;
		//		break;
			} else {
				globuline[i].p = p.x;
				globuline[i].pY = p.y*sin(p.z);
				globuline[i].pZ = p.y*cos(p.z);
				globuline[i].a = Global_acc;
				globuline[i].v = Global_vel;
			}
		}
	}
}

void VesselScene::renderiseee(glm::mat4 &V) {
	glUniformMatrix4fv(V_Id, 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(P_Id, 1, GL_FALSE, &P[0][0]);

	vessel_model->render();

	invPV = glm::inverse(P*V);

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