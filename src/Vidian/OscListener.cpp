#include "OscListener.h"

GestureReceiver::GestureReceiver(GLFWmutex mutex, int *status, int *command, vector<float> *fiability) : osc::OscPacketListener() {
	this->mutex = mutex;
	this->status = status;
	this->command = command;
	this->fiability = fiability;
}

void GestureReceiver::ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint) {
	try{
		//First we look if the message is a broadcast message...
		if(std::strcmp(m.AddressPattern(), "/status") == 0) {
			osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
			const char *message;
			args >> message >> osc::EndMessage;
			if(std::strcmp(message, "tracking ...") == 0) {
				cout << "Person found" << endl;
			}
		//If it's a gesture message we process it.
		} else if(std::strcmp(m.AddressPattern(), "/gesture") == 0) {
			const char *name;
			osc::int32 id;
			float cost;

			osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
			args >> name >> id >> cost >> osc::EndMessage; //we save everything.

			//and get the id of the movement name (if it's not recognised, we shout an error).
			int neu = name2id(name);
			if(neu == -1) {
				global::log.warning("Gesture not recognised received!");
				global::log.warning(name);
			}

			//we check if the fiability is under the maximum allowed.
			//if not we do nothing.
			if((*fiability)[neu] >= cost) {
				//So, WE HAVE A VALID MOVEMENT, let's see what we can do.
				if(*status == 0) { //RUN
					glfwLockMutex(mutex);
						if(neu == 0) *command = 0;
						if(neu == 1) *command = 1;
						if(neu == 2) *command = 2;
					glfwUnlockMutex(mutex);
				} else if(*status == 1) { //STOP
					glfwLockMutex(mutex);
						if(neu == 1 || neu == 2) *command = 0;
						if(neu == 0 || neu == 5) *command = 1;
					glfwUnlockMutex(mutex);
				} else if(*status == 2) { //INFO
					glfwLockMutex(mutex);
						if(neu == 0 || neu == 1 || neu == 2 ) *command = 0;
						if(neu == 5) *command = 1;
					glfwUnlockMutex(mutex);
				}
			}
		}
	} catch( osc::Exception& e ) {
			// any parsing errors such as unexpected argument types, or 
			// missing arguments get thrown as exceptions.
			std::cout << "error while parsing message: " << m.AddressPattern() << ": " << e.what() << "\n";
	}
}

int  GestureReceiver::name2id(const char* name) {
	if(std::strcmp(name, "morepitch") == 0) return 0;
	if(std::strcmp(name, "lesspitch") == 0) return 1;
	if(std::strcmp(name, "pause") == 0)		return 2;
	if(std::strcmp(name, "zoomin") == 0)	return 3;
	if(std::strcmp(name, "zoomout") == 0)	return 4;
	if(std::strcmp(name, "info") == 0)		return 5;
	return -1;
}


void RunGestureReceiver(void *arg) {
	GestureReceiver* listener = (GestureReceiver*)arg;
	UdpListeningReceiveSocket s(IpEndpointName(IpEndpointName::ANY_ADDRESS, PORT), listener);

	s.Run();
}