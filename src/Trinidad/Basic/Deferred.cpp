#include "Deferred.h"


StartDeferred::StartDeferred() {
	first = new Shader("Shaders/Deferred/first.vert", "Shaders/Deferred/first.frag");
	bool calite[] = { true, true, true };
	renderBuffer = new FBO(global::width, global::height, true, 3, calite);
}

void StartDeferred::draw(double t) {
	glDisable(GL_BLEND); //we have to disable blending! (we want alpha channel for other things...)
	renderBuffer->bind();
	first->use();
}


EndDeferred::EndDeferred(StartDeferred *sd) {
	renderBuffer = sd->renderBuffer;
}

void EndDeferred::draw(double t) {
	renderBuffer->unbind();
	glEnable(GL_BLEND); //we enable blending again!
}


RenderDeferred::RenderDeferred(StartDeferred *sd, glm::mat4 *invPV) {
	renderBuffer = sd->renderBuffer;
	second = new Shader("Shaders/Deferred/second.vert", "Shaders/Deferred/second.frag");

	this->invPV = invPV;

	squad = new VBO(global::quad, sizeof(global::quad), 0);
	squad_I = new IBO(global::quad_I, sizeof(global::quad_I));

	normals = second->getUniform("Normal");
	diffuse = second->getUniform("Diffuse");
	specular = second->getUniform("Specular");
	depth = second->getUniform("Depth");
	invPV_id = second->getUniform("invPV");

	Light test(second, "lights");
	test.addDirectionalLight(glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
}

void RenderDeferred::draw(double t) {
	glDisable(GL_DEPTH_TEST);

	second->use();
	renderBuffer->bind_texture(0, 0);
	renderBuffer->bind_texture(1, 1);
	renderBuffer->bind_texture(2, 2);
	renderBuffer->bind_depth_texture(3);

	glUniform1i(normals, 0);
	glUniform1i(diffuse, 1);
	glUniform1i(specular, 2);
	glUniform1i(depth, 3);
	glUniformMatrix4fv(invPV_id, 1, GL_FALSE, &(*invPV)[0][0]);

	squad->enable(3);
	squad_I->draw(GL_TRIANGLES);
	squad->disable();

	glEnable(GL_DEPTH_TEST);
}

DebugDeferred::DebugDeferred(StartDeferred *sd, glm::mat4 *invP) {
	renderBuffer = sd->renderBuffer;
	debug = new Shader("Shaders/Deferred/debug.vert", "Shaders/Deferred/debug.frag");

	this->invP = invP;

	squad = new VBO(global::quad, sizeof(global::quad), 0);
	squad_I = new IBO(global::quad_I, sizeof(global::quad_I));

	tex1 = debug->getUniform("tex1");
	tex2 = debug->getUniform("tex2");
	tex3 = debug->getUniform("tex3");
	tex4 = debug->getUniform("tex4");
	invP_id = debug->getUniform("invPersp");

}

void DebugDeferred::draw(double t) {
	glDisable(GL_DEPTH_TEST);

	debug->use();
	renderBuffer->bind_texture(0, 0);
	renderBuffer->bind_texture(1, 1);
	renderBuffer->bind_texture(2, 2);
	renderBuffer->bind_depth_texture(3);

	glUniform1i(tex1, 0);
	glUniform1i(tex2, 1);
	glUniform1i(tex3, 2);
	glUniform1i(tex4, 3);
	glUniformMatrix4fv(invP_id, 1, GL_FALSE, &(*invP)[0][0]);

	squad->enable(3);
	squad_I->draw(GL_TRIANGLES);
	squad->disable();

	glEnable(GL_DEPTH_TEST);
}