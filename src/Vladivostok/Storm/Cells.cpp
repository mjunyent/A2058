#include "Cells.h"

Cells::Cells(int n, float v, float xRange, float yRange, float zNear, float zFar, float zFarAway, float K, float L, float M) {
	cells = vector<Cell>(n);

	move = true;
	vel = v;
	this->xRange	= xRange;
	this->yRange	= yRange;
	this->zNear		= zNear;
	this->zFar		= zFar;
	this->zFarAway	= zFarAway;
	this->K			= K;
	this->L			= L;
	this->M			= M;

	for(int i=0; i<n; i++) {
		cells[i].p = vec3(randValue(-xRange, xRange),
						  randValue(-yRange, yRange),
						  randValue(std::min(zNear, zFarAway), std::max(zNear, zFarAway)));
		cells[i].v = vec3(0, 0, vel);
	}

	deflector = vec3(0.0, 0.0, 0.0);
}

void Cells::update() {
	if(!move) return;

	for(int i=0; i<cells.size(); i++) {
		cells[i].v -= K*(vec3(0,0,vel) - cells[i].v);

		if(cells[i].p.z > zNear) {
			cells[i].p = vec3(randValue(-xRange, xRange),
						  randValue(-yRange, yRange),
						  randValue(std::min(zFar, zFarAway), std::max(zFar, zFarAway)));

			cells[i].v = vec3(0, 0, vel);
		}
		
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

		vec2 p, pp;
		p  = vec2(cells[i].p.x, cells[i].p.y);
		pp = vec2(deflector.x, deflector.y);
		float l = length(cells[i].p - deflector);
		if(l <= L) {
			p = M/l * (p-pp)/l;
			cells[i].v.x += p.x;
			cells[i].v.y += p.y;
		}
		
		cells[i].p += cells[i].v;	
	}

	sort(cells.begin(), cells.end(), DepthSort() ); 
}

void Cells::readConf(CSParser *csp) {
	vel		 = csp->getf("Cells.Velocity");
	xRange	 = csp->getf("Cells.xRange");
	yRange	 = csp->getf("Cells.yRange");
	zNear	 = csp->getf("Cells.zNear");
	zFar	 = csp->getf("Cells.zFar");
	zFarAway = csp->getf("Cells.zFarAway");
	K		 = csp->getf("Cells.K");
	L		 = csp->getf("Cells.L");
	M		 = csp->getf("Cells.M");
}
