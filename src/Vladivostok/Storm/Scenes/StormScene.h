#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "../../Parser.h"

enum STATE { REST,		 //cells moving, not scanning.
			 DETECTING,  //cells moving, waiting for something to happen.
			 START,		 //cell in range detected, starting scan. Everything Pauses, no grid.
			 GRID,		 //Grid Movement Pattern.
			 STILL,		 //Rendered scan for a few seconds.
			 UNSCAN };	 //Move Back Grid.

class Scanner;

class StormScene {
public:
	StormScene(Scanner *s);
	
	virtual void renderModel() {};

	virtual void textDraw() {};
	virtual void modelDraw() {};
	virtual void linesDraw() {};

//	void readConf(CSParser *csp);

	Scanner *scan;
};