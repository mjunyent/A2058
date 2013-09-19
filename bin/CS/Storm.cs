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
		FocusDistance 250.0
		FStop 2.5
	}
	
	Rig {
		EyeSep 0.0
	}
}

Lights {
	number 0;
}

Storm {
	Size 15.0
	blurStrength 0.0
	DOF {
		COCnear 100
		COCfar  150
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
	StopVel 0.04
}

Scan {
	box {
		left  0
		right 70
		up    -30
		down  40
		near  -120
		far   -130	
	}
	
	restTime 2.0 ;minimum time between end of scan and start of next.
	startTime 1.0 ;time between cell detected and start of grid draw.
	gridVelocity 0.4 ;velocity of the grid to move.
	stillTime 3.0 ;time after rendering the scan.
	
	scanStart 20 ;left displacement of scan (in px.)
	scanSize 120 ;size of the scan (in px.)
	
	deleteRadius 10.5
	
	Grid {
		size 30;
		divisions 4;
	}
	
	scanStartPoint 12.0;
	
	scanTextStart 50
}

Occlusion {
	bias 0.05;
	radius 40;
	linearAtt 2;
	quadraticAtt 5;
}