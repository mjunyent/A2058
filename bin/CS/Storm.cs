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
		FocusDistance 175.0
		FStop 2.5
	}
	
	Rig {
		EyeSep 1.0
	}
}

Lights {
	number 0;
}

Storm {
	Size 15.0
	blurStrength 0.0
	DOF {
		COCnear 150
		COCfar  200
		radiusNear 60
		radiusFar 60
	}
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
		left  0
		right 80
		up    -40
		down  40
		near  -100
		far   -150	
	}
	InterStopsTime 1.0
	StopTime 2.5
	
	Grid {
		size 30;
		divisions 5;
	}
}

Occlusion {
	bias 0.05;
	radius 40;
	linearAtt 2;
	quadraticAtt 5;
}