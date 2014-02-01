#include "Shader.h"


Shader::Shader() {
	numShaders = 0;
	p = 0;
	attributeList.clear();
	uniformList.clear();
}

Shader::~Shader() {
	attributeList.clear();
	uniformList.clear();
	for(int i=0; i<numShaders; i++) glDeleteShader(shaders[i]);
	glDeleteProgram(p);
}

bool Shader::loadFromFile(GLenum type, const char* filename) {
	std::ifstream fp;
	fp.open(filename, std::ios_base::in);
	if(!fp) {
		Tobago.log->write(ERROR) << "Error opening shader file: " << filename;
		return false;
	}

	std::string line, buffer;
	while(std::getline(fp, line)) {
		buffer.append(line);
		buffer.append("\r\n");
	}
	return loadFromString(type, buffer.c_str());
}

bool Shader::loadFromString(GLenum type, const char* source) {
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	if(printShaderInfoLog(shader)) return false;

	shaders[numShaders] = shader;
	numShaders++;
	return true;
}

bool Shader::link() {
	p = glCreateProgram();

	for(int i=0; i<numShaders; i++) glAttachShader(p, shaders[i]);
	glLinkProgram(p);

	if(printProgramInfoLog(p)) return false;

	return true;
}

void Shader::use() {
	glUseProgram(p);
}


void Shader::addUniform(const string& uniform) {
	GLint uid = glGetUniformLocation(p, uniform.c_str());
	if(uid == -1) Tobago.log->write(WARNING) << "Uniform not found: " << uniform;
	uniformList[uniform] = uid;
}

void Shader::addAttribute(const string& attribute) {
	GLint aid = glGetAttribLocation(p, attribute.c_str());
	if(aid == -1) Tobago.log->write(WARNING) << "Attribute not found: " << attribute;
	attributeList[attribute] = aid;
}

GLint Shader::operator[](const string& attribute) {
	return attributeList[attribute];
}

GLint Shader::operator()(const string& uniform) {
	return uniformList[uniform];
}

void Shader::operator()(const string& uniform, GLfloat v0) {
	glUniform1f(uniformList[uniform], v0);
}

void Shader::operator()(const string& uniform, GLint v0) {
	glUniform1i(uniformList[uniform], v0);
}

void Shader::operator()(const string& uniform, glm::vec2 *v2) {
	glUniform2fv(uniformList[uniform], 1, &(*v2)[0]);
}

void Shader::operator()(const string& uniform, glm::vec3 *v3) {
	glUniform3fv(uniformList[uniform], 1, &(*v3)[0]);
}

void Shader::operator()(const string& uniform, glm::vec4 *v4) {
	glUniform4fv(uniformList[uniform], 1, &(*v4)[0]);
}

void Shader::operator()(const string& uniform, glm::mat2 *m2) {
	glUniformMatrix2fv(uniformList[uniform], 1, GL_FALSE, &(*m2)[0][0]);
}

void Shader::operator()(const string& uniform, glm::mat3 *m3) {
	glUniformMatrix3fv(uniformList[uniform], 1, GL_FALSE, &(*m3)[0][0]);
}

void Shader::operator()(const string& uniform, glm::mat4 *m4) {
	glUniformMatrix4fv(uniformList[uniform], 1, GL_FALSE, &(*m4)[0][0]);
}

bool Shader::printShaderInfoLog(GLuint shader) {
    int infologLength = 0;
    int charsWritten  = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);
	
    if (infologLength > 1)
    {
        GLchar *infoLog = new GLchar[infologLength];
        glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		Tobago.log->write(ERROR) << "Shader compiling error:" << infoLog;
        delete [] infoLog;
		return true;
    }

	return false;
}

bool Shader::printProgramInfoLog(GLuint program) {
    int infologLength = 0;
    int charsWritten  = 0;
    
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 1)
    {
		GLchar *infoLog = new GLchar[infologLength];
        glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
		Tobago.log->write(ERROR) << "Shader program linking error:" << infoLog;
		delete [] infoLog;
		return true;
    }

	return false;
}
