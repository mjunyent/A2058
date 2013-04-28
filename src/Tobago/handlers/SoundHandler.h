#ifndef NO_SOUND

#pragma once
#include <iostream>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/glfw.h>

#include "fmod.hpp"
#include "fmod_errors.h"

/*
	FMOD Ex API :
		- fmodex_vc.lib
		- fmod.hpp
		- fmodex.dll
*/
void initSound();

class SoundHandler{
	public :
		float* FFT;
		float* WAVE;
		unsigned len; //Con el array y su longitud publicos se accede a los datos que pilles usando el getSpectrum ...
		FMOD::Sound* theSound;
		FMOD::Channel* thechannel;
		float playFreq;

		SoundHandler(char *SongFile, unsigned FFTLEN);
		void Play();
		void getSpectrum();
		void getWave();
		double SoundTime();
		void PlotWave();

		double getEnergy();

		void setVel(float v); //Play needs to be called at some point before this.
};

#endif