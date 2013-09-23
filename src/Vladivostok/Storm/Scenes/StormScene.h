#pragma once

#include "Tobago/Tobago.h"
#include "Trinidad/Trinidad.h"
#include "../../Parser.h"

class Scanner;

class StormScene {
public:
	StormScene(Scanner *s);
	
	virtual void renderModel() {};

	virtual void textDraw() {};
	virtual void modelDraw() {};
	virtual void linesDraw() {};

	virtual void StatusUpdate() {};

//	void readConf(CSParser *csp);

	Scanner *scan;
};