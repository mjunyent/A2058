#include "FBO.h"

FBO::FBO(GLsizei width, GLsizei height, bool dbo, int ntbo, bool *qualite) 
{
	this->width = width;
	this->height = height;

	int maxDrawBuffers;
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);
	if(maxDrawBuffers <= ntbo) {
		TOBAGO::log.write(WARNING) << "FBO: Be careful not to exceed MAX_DRAW_BUFFERS number!";
	}

	glGenFramebuffers(1, &theID);
	glBindFramebuffer(GL_FRAMEBUFFER, theID);

	//Create and attach color textures.
	this->ntbo = ntbo;
	textures.resize(ntbo);

	for(int i=0; i<ntbo; i++) {
		textures[i] = new TBO(GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, 0, qualite[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, textures[i]->theID, 0);
	}

	if(dbo) {
		depthtexture = new TBO(GL_DEPTH_COMPONENT24, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, 0, false);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,  depthtexture->theID, 0);
	}

	GLenum *DrawBuffers = new GLenum[ntbo];
	for(int i=0; i<ntbo; i++) DrawBuffers[i] = GL_COLOR_ATTACHMENT0+i;
	glDrawBuffers(ntbo, DrawBuffers); // "ntbo" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	GLenum check_result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(check_result != GL_FRAMEBUFFER_COMPLETE) {
		shout_error(check_result);
		status = false;
	} else status = true;

	if(!status) TOBAGO::log.write(ERROR) << "Error creating FBO";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO::FBO(GLsizei width, GLsizei height, vector<TBO*> texs, TBO *depth, bool *qualite) 
{
	this->width = width;
	this->height = height;
	this->ntbo = texs.size();

	int maxDrawBuffers;
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);
	if(maxDrawBuffers <= ntbo) {
		TOBAGO::log.write(WARNING) << "FBO: Be careful not to exceed MAX_DRAW_BUFFERS number!";
	}

	glGenFramebuffers(1, &theID);
	glBindFramebuffer(GL_FRAMEBUFFER, theID);

	depthtexture = NULL;

	for(unsigned int i=0; i<texs.size(); i++) {
		texs[i]->load(GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, 0, qualite[i]);
	}
	if(depth != NULL) depth->load(GL_DEPTH_COMPONENT24, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, 0, false);

	// Set "renderedTexture" as our colour attachement #0
	for(unsigned int i=0; i<texs.size(); i++) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, texs[i]->theID, 0);
	}
	if(depth != NULL) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,  depth->theID, 0);

	// Set the list of draw buffers.
	GLenum *DrawBuffers = new GLenum[ntbo];
	for(int i=0; i<ntbo; i++) DrawBuffers[i] = GL_COLOR_ATTACHMENT0+i;
	glDrawBuffers(ntbo, DrawBuffers); // "ntbo" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	GLenum check_result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(check_result != GL_FRAMEBUFFER_COMPLETE) {
		shout_error(check_result);
		status = false;
	} else status = true;

	if(!status) TOBAGO::log.write(ERROR) << "Error creating FBO";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::bind() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, theID);				//bind buffer
	glGetIntegerv(GL_VIEWPORT, viewport);
//	glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);			//push viewport and enable config
	glViewport(0, 0, width, height);						//set viewport
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clean things
}

void FBO::unbind() 
{
//	glPopAttrib();											//pop config
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);	//set viewport
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::erase() {
	for(unsigned int i=0; i<textures.size(); i++) textures[i]->erase();
	if(depthtexture != NULL) depthtexture->erase();
//	glDeleteRenderbuffers(1, &depthrenderbuffer);
	glDeleteFramebuffers(1, &theID);
}

void FBO::bind_texture(int texture, int id) 
{
	textures[texture]->bind(id);
}

void FBO::bind_depth_texture(int id)
{
	depthtexture->bind(id);
}


void FBO::shout_error(GLenum error) {
	switch (error)
	{
		case GL_FRAMEBUFFER_UNDEFINED:
			TOBAGO::log.write(ERROR) << "FBO Creation error: GL_FRAMEBUFFER_UNDEFINED";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			TOBAGO::log.write(ERROR) << "FBO Creation error: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			TOBAGO::log.write(ERROR) << "FBO Creation error: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			TOBAGO::log.write(ERROR) << "FBO Creation error: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			TOBAGO::log.write(ERROR) << "FBO Creation error: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			TOBAGO::log.write(ERROR) << "FBO Creation error: GL_FRAMEBUFFER_UNSUPPORTED";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			TOBAGO::log.write(ERROR) << "FBO Creation error: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			TOBAGO::log.write(ERROR) << "FBO Creation error: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
			break;
		case GL_FRAMEBUFFER_COMPLETE:
			TOBAGO::log.write(ERROR) << "FBO Creation ok, why is this in LOG?";
			break;
		case 0:
			TOBAGO::log.write(ERROR) << "FBO Creation error: 0 returned";
		default:
			TOBAGO::log.write(ERROR) << "FBO Creation error: Error not recognised";
		break;
	}
	/* http://www.opengl.org/sdk/docs/man3/xhtml/glCheckFramebufferStatus.xml
	GL_FRAMEBUFFER_UNDEFINED is returned if target is the default framebuffer, but the default framebuffer does not exist.

	GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT is returned if any of the framebuffer attachment points are framebuffer incomplete.

	GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT is returned if the framebuffer does not have at least one image attached to it.

	GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER is returned if the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAWBUFFERi.

	GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER is returned if GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER.

	GL_FRAMEBUFFER_UNSUPPORTED is returned if the combination of internal formats of the attached images violates an implementation-dependent set of restrictions.

	GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE is returned if the value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES.

	GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE is also returned if the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures.

	GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS is returned if any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target.
	*/
}