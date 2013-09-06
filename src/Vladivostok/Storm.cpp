#include "Storm.h"

Storm::Storm(CSParser *csp) {
	this->csp = csp;


	screenQuad = new VBO(director::quad, sizeof(director::quad), 0);
	screenQuad_I = new IBO(director::quad_I, sizeof(director::quad_I));


	myCam = new Camera(csp->data.width, csp->data.height,
					   csp->data.FOV, csp->data.zNear, csp->data.zFar,
					   csp->data.CamPos, csp->data.CamDir, csp->data.CamUp,
					   csp->data.FocalLength, csp->data.FocusDistance, csp->data.FStop);

	myRig = new Rig(*myCam, csp->data.EyeSep);

	billboardShad = new Shader("Shaders/Vladivostok/storm.vert", "Shaders/Vladivostok/storm.geom", "Shaders/Vladivostok/storm.frag");
	billboard_M_Id = billboardShad->getUniform("Model");
	billboard_V_Id = billboardShad->getUniform("View");
	billboard_P_Id = billboardShad->getUniform("Projection");
	billboard_up_id = billboardShad->getUniform("up");
	billboard_camPos_Id = billboardShad->getUniform("camPos");
	billboard_r_Id = billboardShad->getUniform("r");
}

void Storm::draw(int s, double t) {

	billboardShad->use();

	mat4 idd = translate(0.0f, 0.0f, 0.0f);

	glUniformMatrix4fv(billboard_M_Id, 1, GL_FALSE, &idd[0][0]); 
	glUniformMatrix4fv(billboard_V_Id, 1, GL_FALSE, &myCam->V[0][0]);
	glUniformMatrix4fv(billboard_P_Id, 1, GL_FALSE, &myCam->P[0][0]);
	glUniform3fv(billboard_up_id, 1, &myCam->up[0]);
	glUniform3fv(billboard_camPos_Id, 1, &myCam->position[0]);
	glUniform1f(billboard_r_Id, 0.2);


	screenQuad->enable(3);
	screenQuad->draw(GL_POINTS);
//	screenQuad_I->draw(GL_TRIANGLES);
	screenQuad->disable();
}

void Storm::update(double t) {
	csp->parse();
	csp->passToCam(myCam);
}