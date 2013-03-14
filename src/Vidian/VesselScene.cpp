#include "VesselScene.h"

VesselScene::VesselScene(Shader *shader, glm::mat4 *V) {
	vessel = new A3dsHandler("Models/vessel.3ds", 1);
	vessel->calculateNormals();

	cell = new A3dsHandler("Models/cell.3ds", 0);
	cell->calculateNormals();

	P = glm::perspective(75.0f, 4.0f/3.0f, 0.1f, 100.0f);
	this->rig = NULL;
	this->V = V;
	M_Vessel = glm::rotate(90.0f, glm::vec3(1, 0, 0));

	for(int i=0; i<12; i++) {
	//	glm::mat4 r = glm::rotate(float(rand()%180), rand()%1000/1000.0f,rand()%1000/1000.0f,rand()%1000/1000.0f);
		glm::mat4 t = glm::translate(rand()%1000/200.0f-3.2f, -1.5f+rand()%1000/1000.0f, 6.0f+rand()%1000/1000.0f);

		M_Cells.push_back(t);
	}

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
	//	glm::mat4 r = glm::rotate(float(rand()%180), rand()%1000/1000.0f,rand()%1000/1000.0f,rand()%1000/1000.0f);
		glm::mat4 t = glm::translate(rand()%1000/200.0f-3.2f, -1.5f+rand()%1000/1000.0f, 6.0f+rand()%1000/1000.0f);

		M_Cells.push_back(t);
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


void VesselScene::renderiseee(glm::mat4 &V) {
	glUniformMatrix4fv(M_Id, 1, GL_FALSE, &M_Vessel[0][0]);
	glUniformMatrix4fv(V_Id, 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(P_Id, 1, GL_FALSE, &P[0][0]);

	glUniform1f(Scale_Id, 1.0f);

	vessel->vertexs->enable(3);
	vessel->normals->enable(3);
	vessel->indexs->draw(GL_TRIANGLES);
	vessel->vertexs->disable();
	vessel->normals->disable();

	glUniform1f(Scale_Id, 0.3f);
	cell->vertexs->enable(3);
	cell->normals->enable(3);
	for(int i=0; i<M_Cells.size(); i++) {
		glm::mat4 M_Cell = M_Cells[i];
		glUniformMatrix4fv(M_Id, 1, GL_FALSE, &M_Cell[0][0]);
	
		cell->indexs->draw(GL_TRIANGLES);	
	}
	cell->vertexs->disable();
	cell->vertexs->disable();	
}