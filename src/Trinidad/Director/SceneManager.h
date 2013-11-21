#pragma once
#ifndef TRINIDAD_SCENEMANAGER
#define TRINIDAD_SCENEMANAGER

#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <list>
#include <algorithm>
#include "Tobago/Tobago.h"

using namespace std;

/*class MyScene : public Scene {
	MyScene(cosas) {
	}

	void draw(double t, int s) {
	}

	void update(double t) {
	}
};*/

class Scene {
public:
	bool started;
	bool ended;

	int priority;

	double start;
	double end;

	FBO *inputBuffL, *inputBuffR;	//JUST A POINTER TO LAST SCENE OUTPUT FBO.
	FBO *outputBuffL, *outputBuffR; //JUST A POINTER TO CURRENT SCENE OUTPUT FBO. Use it to save your output.

	Scene() {};
	virtual void pre() {};						//function called before first draw (in render loop).
	virtual void post() {};						//function called after last draw (in render loop).
	virtual void draw(int s, double t) {};		//function called during the render loop.
	virtual void update(double t) {};			//function called when update.
};

static bool sort_scene(list<Scene*>::iterator i, list<Scene*>::iterator j) { //lowest priority comes first.
	return ((*i)->priority < (*j)->priority);
}

class SceneManager {
public:
	SceneManager(double *t); //number of windows is the director windows size.
	SceneManager(double *t, int maxWindows);

	void addScene(Scene *s,			//scene to render
				  double start,		//start second
				  double end,		//end second
				  float priority	//priority (lower execute first!)		
				 );

	void render(int w = -1);

	void update();

private:
	double *time;
	int maxWindows;
	list<Scene*> scenes;
};

#endif