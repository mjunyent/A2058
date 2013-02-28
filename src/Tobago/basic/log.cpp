#include "log.h"

using namespace std;

Log::Log(char* name) {
    filename = name;

	fstream IN;
	IN.open(filename);
	

   output = new ofstream;
    (*output).open(filename);
    if (!(*output)) {
        cerr << "Unable to open file " << filename << endl;
    }
	

    *output << endl << endl  << "A2058 \t Error Log \t " << filename << endl  << endl;
}

void Log::error(const char* message) {
    *output << glfwGetTime() << "\t ERROR: " << message << endl;
}

void Log::warning(const char* message) {
    *output << glfwGetTime() << "\t WARNING: " << message << endl;
}

void Log::message(const char* message) {
    *output << glfwGetTime() << "\t MESSAGE: " << message << endl;
}

void Log::close() {
    *output << glfwGetTime() << "\t ****** \t CLOSSING LOG \t ******" << endl;
    (*output).close();
    
}