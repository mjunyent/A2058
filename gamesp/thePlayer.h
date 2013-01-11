#pragma once
#include "./thePreprocessor.h"


/*
	1. Evaluar por constantes en shader ?
	1. Evaluar por constantes en CPU ?
	Requiere modificar VBO -> palo enorme !
	¿Como guardo la posicion actual?
	Computando matriz de traslacion + shader = no modifica VBO + existen datos consultables para evaluar.
*/

class ThePlayer
{
	public :
		ThePlayer(); //centro de pantalla + offset.
		void eval_move();
		VBO square;
		IBO theIBO;
	private :

};