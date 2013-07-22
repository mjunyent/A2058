#include "SceneManager.h"
#include "global.h"

SceneManager::SceneManager(double *t) {
	time = t;
	this->maxWindows = DIRECTOR::windows.size();
}

SceneManager::SceneManager(double *t, int maxWindows) {
	time = t;
	this->maxWindows = maxWindows;
}

void SceneManager::addScene(Scene *s, double start, double end, float priority) {
	s->start = start;
	s->end = end;
	s->priority = priority;
	s->started = false;
	s->ended = false;
	scenes.push_back(s);
}

void SceneManager::render(int w) {
	vector<list<Scene*>::iterator> render_pipeline;
	for(list<Scene*>::iterator it=scenes.begin(); it != scenes.end(); it++) {
		if((*it)->start <= *time && (*it)->end >= *time) { //event is inside it's time range, push it to queue.
			render_pipeline.push_back(it);
		} else if((*it)->ended == false && (*it)->end < *time) { //if end already happened but ended flag is false, end it.
			(*it)->post();
			(*it)->ended = true;
		}
		//maybe we could delete ended events instead of saving them...
	}
	sort(render_pipeline.begin(), render_pipeline.end(), sort_scene);

	if(w == -1) {
		for(int win=0; win<maxWindows; win++) {
			glfwMakeContextCurrent(DIRECTOR::windows[win]);
			for(int i=0; i<render_pipeline.size(); i++) {
				if((*render_pipeline[i])->started == false) { //if it's first call (not started), call pre function.
					(*render_pipeline[i])->pre();
					(*render_pipeline[i])->started = true;
				}
				(*render_pipeline[i])->draw( win, (*time - (*render_pipeline[i])->start) / ((*render_pipeline[i])->end - (*render_pipeline[i])->start) );
			}
		}
					glfwSwapBuffers(DIRECTOR::windows[0]);
					glfwSwapBuffers(DIRECTOR::windows[1]);
	} else {
		for(int i=0; i<render_pipeline.size(); i++) {
				if((*render_pipeline[i])->started == false) { //if it's first call (not started), call pre function.
					(*render_pipeline[i])->pre();
					(*render_pipeline[i])->started = true;
				}
				(*render_pipeline[i])->draw( w, (*time - (*render_pipeline[i])->start) / ((*render_pipeline[i])->end - (*render_pipeline[i])->start) );
			}
	}
}

void SceneManager::update() {
	vector<list<Scene*>::iterator> update_pipeline;

	for(list<Scene*>::iterator it=scenes.begin(); it != scenes.end(); it++) {
		if((*it)->start <= *time && (*it)->end >= *time && (*it)->ended == false) {
			update_pipeline.push_back(it);
		}
	}

	sort(update_pipeline.begin(), update_pipeline.end(), sort_scene);

	for(int i=0; i<update_pipeline.size(); i++) {
		(*update_pipeline[i])->update( (*time - (*update_pipeline[i])->start) / ((*update_pipeline[i])->end - (*update_pipeline[i])->start) );
	}
}
