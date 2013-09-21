#include "Cells.h"

Cells::Cells(int n, CSParser *csp) {
	cells = vector<Cell>(n);
	sortedCells = vector<Cell*>(n);

	Play();
	readConf(csp);

	for(int i=0; i<n; i++) {
		cells[i].id = i;
		cells[i].p = vec3(randValue(-xRange, xRange),
						  randValue(-yRange, yRange),
						  randValue(std::min(zNear, zFarAway), std::max(zNear, zFarAway)));
		cells[i].v = vec3(0, 0, vel);
		cells[i].alpha = 1.0;

		sortedCells[i] = &cells[i];
	}

	selectedCell = -1;
}

void Cells::slowStop(int cell) {
	reduce = true;
	reduceId = cell;
}

void Cells::select(int cell) {
	selectedCell = cell;
//	cells[selectedCell].v = vec3(0.0);
	cells[selectedCell].alpha = 0.0;
}

void Cells::Play() {
	move = true;
	reduce = false;
	reduceMult = 1.0;
}

void Cells::update() {
	if(reduce || !move) {
		for(int i=0; i<cells.size(); i++) {
			if(cells[i].p.z < zNear && cells[i].p.z > cells[reduceId].p.z) cells[i].p += cells[i].v;
		}
	}
	if(reduce) {
		reduceMult -= reduceVel;
		if(reduceMult <= 0) {
			reduce = false;
			move = false;
		}
	}
	if(!move) return;

	for(int i=0; i<cells.size(); i++) {
		if(i == selectedCell) continue;
		//Calculate the plane deacceleration
		cells[i].v -= K*(vec3(0,0,vel) - cells[i].v);

		//Reposition too near cells
		if(cells[i].p.z > zNear) {
			cells[i].p = vec3(randValue(-xRange, xRange),
						  randValue(-yRange, yRange),
						  randValue(std::min(zFar, zFarAway), std::max(zFar, zFarAway)));
			cells[i].alpha = 1.0;
			cells[i].v = vec3(0, 0, vel);
		}
		
		//If no reduce, separate too near cells.
		if(!reduce) {
			for(int j=0; j<cells.size(); j++) {
				if(j != i) {
					vec2 p, pp;
					p  = vec2(cells[i].p.x, cells[i].p.y);
					pp = vec2(cells[j].p.x, cells[j].p.y);
					float l = length(p - pp);
					if(l <= L) {
						p = M/l * (p-pp)/l;
						cells[i].v.x += p.x;
						cells[i].v.y += p.y;
					}
				}
			}
		}

		//Camera deflector
		vec2 p, pp;
		p  = vec2(cells[i].p.x, cells[i].p.y);
		pp = vec2(deflector.x, deflector.y);
		float l = length(cells[i].p - deflector);
		if(l <= deflectorL) {
			p = deflectorM/l * (p-pp)/l;
			cells[i].v.x += p.x;
			cells[i].v.y += p.y;
		}

		//Center Attractor.
		p  = vec2(cells[i].p.x, cells[i].p.y);
		pp = vec2(deflector.x, deflector.y);
		l = length(p - pp);
		p = H*(p-pp);
		cells[i].v.x += p.x;
		cells[i].v.y += p.y;

		cells[i].p += reduceMult*cells[i].v;
		cells[i].alpha = clamp(cells[i].alpha-alphaVel, 0.0f, 1.0f);
	}

	if(selectedCell != -1) {
		float celerity = length(cells[selectedCell].v);
		vec3 direction = centerPosition-cells[selectedCell].p;
		cells[selectedCell].v = normalize(cells[selectedCell].v + centerVelocity*direction)*celerity;
		if(length(cells[selectedCell].p - centerPosition) > centerUncertainty) {
			cells[selectedCell].p += reduceMult*cells[selectedCell].v;
		} else {
			reduce = true;
			reduceId = selectedCell;
			selectedCell = -1;
		}
	}

	sort(sortedCells.begin(), sortedCells.end(), DepthSort() ); 
}

void Cells::readConf(CSParser *csp) {
	vel		 = csp->getf("Cells.Velocity");
	xRange	 = csp->getf("Cells.xRange");
	yRange	 = csp->getf("Cells.yRange");
	zNear	 = csp->getf("Cells.zNear");
	zFar	 = csp->getf("Cells.zFar");
	zFarAway = csp->getf("Cells.zFarAway");
	H		 = csp->getf("Cells.H");
	K		 = csp->getf("Cells.K");
	L		 = csp->getf("Cells.L");
	M		 = csp->getf("Cells.M");
	reduceVel = csp->getf("Cells.StopVel");
	alphaVel = csp->getf("Cells.alphaVel");

	deflector = csp->toVec3(csp->pr.get<std::string>("Cells.Deflector.position"));
	deflectorL = csp->getf("Cells.Deflector.L");
	deflectorM = csp->getf("Cells.Deflector.M");

	centerVelocity = csp->getf("Cells.centerVelocity");
	centerPosition = csp->toVec3(csp->pr.get<std::string>("Cells.centerPosition"));
	centerUncertainty = csp->getf("Cells.centerUncertainty");

}
