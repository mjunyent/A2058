#include "Tobago/Tobago.h" //Tobago contains basic opengl inicialitzation, object managers and everything you need to ease your code.
#include "Trinidad/Trinidad.h" //Trinidad contains scenes already done, just to plug & play to an sceneManager or wherever.
#include "Vidian/VesselScene.h"

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

#define PORT 7000

class ExamplePacketListener : public osc::OscPacketListener {
protected:

    virtual void ProcessMessage( const osc::ReceivedMessage& m, 
				const IpEndpointName& remoteEndpoint )
    {
        (void) remoteEndpoint; // suppress unused parameter warning

        try{
            // example of parsing single messages. osc::OsckPacketListener
            // handles the bundle traversal.
            
            if( std::strcmp( m.AddressPattern(), "/test1" ) == 0 ){
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                bool a1;
                osc::int32 a2;
                float a3;
                const char *a4;
                args >> a1 >> a2 >> a3 >> a4 >> osc::EndMessage;
                
                std::cout << "received '/test1' message with arguments: "
                    << a1 << " " << a2 << " " << a3 << " " << a4 << "\n";
                
            }else if( std::strcmp( m.AddressPattern(), "/test2" ) == 0 ){
                // example #2 -- argument iterator interface, supports
                // reflection for overloaded messages (eg you can call 
                // (*arg)->IsBool() to check if a bool was passed etc).
                osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
                bool a1 = (arg++)->AsBool();
                int a2 = (arg++)->AsInt32();
                float a3 = (arg++)->AsFloat();
                const char *a4 = (arg++)->AsString();
                if( arg != m.ArgumentsEnd() )
                    throw osc::ExcessArgumentException();
                
                std::cout << "received '/test2' message with arguments: "
                    << a1 << " " << a2 << " " << a3 << " " << a4 << "\n";
            }
        }catch( osc::Exception& e ){
            // any parsing errors such as unexpected argument types, or 
            // missing arguments get thrown as exceptions.
            std::cout << "error while parsing message: "
                << m.AddressPattern() << ": " << e.what() << "\n";
        }
    }
};

void RunListener(void *arg) {
	
    ExamplePacketListener listener;
    UdpListeningReceiveSocket s(
            IpEndpointName( IpEndpointName::ANY_ADDRESS, PORT ),
            &listener );
	s.RunUntilSigInt();

}

int main(void) {
	int running = true;

	OGL::init(1280, 720, 0, 0, 0, 32, 64, 1, "A2058", 3, 4, GLFW_WINDOW);
//	glfwSwapInterval(20);

	cout << "WTF" << endl;
	glfwCreateThread(RunListener, NULL);

	//Initialize some vars...
	global::song = new SoundHandler("hearthShort5.mp3", 1024);
	global::manager = new SceneManager(&global::currentTime);
	Timer timer(global::manager);

	//Initialize scenes
	Camera cam(glm::vec3(2.4f,-1, 5.16f), glm::vec3(-0.99, 0.0556, 0.123), glm::vec3(0, 1, 0), 0.05, true);
	StartDeferred Sdeferred;
	EndDeferred Edeferred(&Sdeferred);
	VesselScene vessel(Sdeferred.first, &cam.V);
	RenderDeferred Rdeferred(&Sdeferred, &vessel.invPV, &cam.position);

	Light *lights = Rdeferred.lights;

	//Add lights! (maybe this should go inside vessel.
	lights->addDirectionalLight(glm::vec3(2, 0.0, 0.0),	-cam.direction, glm::vec3(1.0,1.0,1.0));
//	lights->addPointLight(cam.position, glm::vec3(2, 0.2, 0.05), glm::vec3(1.0, 1.0, 1.0));

//	lights->addSpotLight(cam.position, glm::vec3(2, 0.2, 0.05), cam.direction, glm::vec3(1.0, 1.0, 1.0), cos(25.0 * 3.141592 / 180.0), cos(15.0 * 3.141592 / 180.0), 2);
//	lights->addDirectionalLight(glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));

//	DebugDeferred Ddeferred(&Sdeferred, &vessel.invPV);

	global::manager->addScene(&cam,			4,		100000000,		0);
	global::manager->addScene(&Sdeferred,	4,		100000000,		1);
	global::manager->addScene(&vessel,		4,		100000000,		4);
	global::manager->addScene(&Edeferred,	4,		100000000,		5);
	global::manager->addScene(&Rdeferred,	4,		100000000,		6);
//	global::manager->addScene(&Ddeferred,	0,		100000000,		7);
	global::manager->addScene(new SoundSpectrum, 0, 100000000,			  20000);
	global::manager->addScene(new FrameRate(5, 5, 200, 50), 0, 100000000, 20001);

	global::song->Play();

	while(running) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		timer.update();
		global::manager->render();
		glfwSwapBuffers();
		randValue(100, 200);
		if(glfwGetKey( 'Z' )) global::song->setVel(global::song->playVel - 0.05);
		if(glfwGetKey( 'X' )) global::song->setVel(global::song->playVel + 0.05);
		//cout << global::song->playVel << endl;
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

	glfwTerminate();
	return 0;
}