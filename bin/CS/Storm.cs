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
		zFar 500.0
	}
	
	Camera {
		Position 0.0,0.0,104.0
		Direction 0.0,0.0,-1.0
		Up 0.0,1.0,0.0	
	}
	
	Lens {
		FocalLength  22.0
		FocusDistance 60.0
		FStop 2.5
	}
	
	Rig {
		EyeSep 0.2
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
	Velocity 1.5
	zMax 100.0
	Size 15.0
	texSize 0.20
	yMargin 80
	xMargin 125
	zSpawnMin -520
	zSpawnMax -500
}

Box {
	scale 1.0;
	shininess 0.45;
}

Occlusion {
	bias 0.05;
	radius 40;
	linearAtt 2;
	quadraticAtt 5;

}