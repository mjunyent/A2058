Setup {
	Screen {
		width 1820   ; 1920
		height 1000  ; 1080
		title MyWindow
	}

	OpenGL {
		Major 4
		Minor 2
	}
}

View {
	Perspective {
		FOV 45.0
		zNear 6.0
		zFar 600.0
	}
	
	Camera {
		Position 0.0,0.0,10.0
		Direction 0.0,0.0,-1.0
		Up 0.0,1.0,0.0	
	}
	
	Lens {
		FocalLength  50.0
		FocusDistance 240.0
		FStop 2.5
	}
	
	Rig {
		EyeSep 1.0
	}
}

Lights {
	number 0;
}

TestSphere {
	Position 0.0,0.0,92.0
}

Spheres {
	RenderBox 2.0 ; if bigger than one render, else, doesnt.
	Size 15.0
	texSize 0.20
	
	
	zMax 100.0

	Velocity 0.0
	yMargin 80
	xMargin 125
	zSpawnMin -520
	zSpawnMax -500
}

Cells {
	Velocity 1.0
	xRange 125
	yRange 80
	zNear 10
	zFar -600
	zFarAway -700
	K -0.2
	M 2
	L 50
}

Scan {
	box {
		left 40
		right 120
		up -60
		down 60
		near -200
		far -250	
	}
}










Occlusion {
	bias 0.05;
	radius 40;
	linearAtt 2;
	quadraticAtt 5;
}