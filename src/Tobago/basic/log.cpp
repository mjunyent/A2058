#include "log.h"

using namespace std;

Log::Log(const char* name) {
	m_out = new ofstream;
	(*m_out).open(name);
	if(!(*m_out)) cerr << "Unable to open file " << name << endl;
	(*m_out) << "A2058 \t Error Log \t " << name << endl;
}

Log::~Log() {
	delete m_out;
}

ofstream& Log::write(LogLevel ERRNO) {
	(*m_out) << endl << glfwGetTime() << "\t ";
	if(ERRNO == ERROR)			(*m_out) << "ERROR";
	else if (ERRNO == WARNING) 	(*m_out) << "WARNING";
	else if (ERRNO == INFO) 	(*m_out) << "INFO";
	else if (ERRNO == DEBUG) 	(*m_out) << "DEBUG";
	(*m_out) << ": ";
	return (*m_out);
}

namespace TOBAGO {
	Log log = Log("logTobago.txt");
}