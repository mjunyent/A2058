#pragma once
#ifndef VIDIAN_OSCLISTENER
#define VIDIAN_OSCLISTENER

#include "../Tobago/Tobago.h"
#include "../Trinidad/Trinidad.h"

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

#define PORT 8000

class GestureReceiver : public osc::OscPacketListener {
public:
	GLFWmutex mutex;
	int* status;
	int* command;
	vector<float> *fiability;

	GestureReceiver(GLFWmutex mutex, int *status, int *command, vector<float> *fiability);

	int  name2id(const char* name);

protected:
	virtual void ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint);
};

void RunGestureReceiver(void *arg);


#endif