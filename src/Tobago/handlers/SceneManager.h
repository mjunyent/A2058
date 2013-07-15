#pragma once

#ifndef Scene_manager_h
#define Scene_manager_h

#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <list>
#include <algorithm>

using namespace std;

/*class MyScene : public Scene {
	MyScene(cosas) {
	}

	void draw(double t) {
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

	Scene() {};
	virtual void pre() {};				//function called before first draw (in render loop).
	virtual void post() {};				//function called after last draw (in render loop).
	virtual void draw(double t) {};		//function called during the render loop.
	virtual void update(double t) {};	//function called when update.
};

static bool sort_scene(list<Scene*>::iterator i, list<Scene*>::iterator j) { //lowest priority comes first.
	return ((*i)->priority < (*j)->priority);
}

class SceneManager {
public:
	SceneManager(double *t);

	void addScene(Scene *s,			//scene to render
				  double start,		//start second
				  double end,		//end second
				  float priority	//priority (lower execute first!)		
				 );

	void render();

	void update();

private:
	double *time;
	list<Scene*> scenes;
};

#endif