#include "Deferred.h"


StartDeferred::StartDeferred() {
	first = new Shader("Shaders/Deferred/first.vert", "Shaders/Deferred/first.frag");
	bool calite[] = { true, true, true, true };
	renderBuffer = new FBO(global::width, global::height, false, 4, calite);
}

void StartDeferred::draw(double t) {
	renderBuffer->bind();
	first->use();
}

EndDeferred::EndDeferred(StartDeferred *sd) {
	renderBuffer = sd->renderBuffer;
}

void EndDeferred::draw(double t) {
	renderBuffer->unbind();
}



DebugDeferred::DebugDeferred(StartDeferred *sd) {
	renderBuffer = sd->renderBuffer;
	debug = new Shader("Shaders/Deferred/debug.vert", "Shaders/Deferred/debug.frag");

	squad = new VBO(global::quad, sizeof(global::quad), 0);
	squad_I = new IBO(global::quad_I, sizeof(global::quad_I));

	tex1 = debug->getUniform("tex1");
	tex2 = debug->getUniform("tex2");
	tex3 = debug->getUniform("tex3");
	tex4 = debug->getUniform("tex4");

}

void DebugDeferred::draw(double t) {
	glDisable(GL_DEPTH_TEST);

	debug->use();
	renderBuffer->bind_texture(0, 0);
	renderBuffer->bind_texture(1, 1);
	renderBuffer->bind_texture(2, 2);
	renderBuffer->bind_texture(3, 3);

	glUniform1i(tex1, 0);
	glUniform1i(tex2, 1);
	glUniform1i(tex3, 2);
	glUniform1i(tex4, 3);

	squad->enable(3);
	squad_I->draw(GL_TRIANGLES);
	squad->disable();

	glEnable(GL_DEPTH_TEST);
}