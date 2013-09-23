Setup {
	Screen {
		width 2560   ; 1920
		height 720  ; 1080
		title MyWindow
	}

	OpenGL {
		Major 4
		Minor 2
	}
}

View {
	Perspective {
		FOV 25.0
		zNear 6.0
		zFar 700.0
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
		EyeSep 2.0 ;1.0
	}
}

Lights {
	number 0;
}

Storm {
	Size 15.0
	blurStrength 0.0
	DOF {
		COCnear 70
		COCfar  140
		radiusNear 60
		radiusFar 60
	}
}

Cells {
	Velocity 1.0	;Depth velocity of cells.
	xRange 400		;New cells will appear in a -xRange to xRange x position.
	yRange 80		;New cells will appear in a -yRange to yRange y position.
	zNear 10		;New cells will disappear to reappear when they cross zNear.
	zFar -500		;New cells will appear in a zFar to zFar away range.
	zFarAway -700
	H -0.0001		;Constant that controls the attraction to the center. KEEP IT LOOOOW!
	K -0.2			;Constant that controls the deacceleration of cells in x,y coordinates.
	M 2				;Constant that controls the speed in which cells separate from each other.
	L 50			;Minimum distance for cells to reppeal.
	StopVel 0.02	;Acceleration to stop.
	alphaVel 0.015	;Fadding in velocity.
	
	centerVelocity 0.001
	centerPosition 50.0,0.0,-135.0
	centerUncertainty 10

	Deflector {
		M 12
		L 100
		position 0,0,0
	}
	
	Play {
		MultMax 4.0
		MultIncr 0.1
		MultDecr 0.05
	}
}

Scan {
	box {
		left  -60
		right 50
		up    -25
		down  25
		near  -180 ;-130
		far   -200 ;-140	
	}
	
	restTime 4.0 ;minimum time between end of scan and start of next.
	startTime 1.0 ;time between cell detected and start of grid draw.
	gridVelocity 1.0 ;velocity of the grid to move.
	stillTime 10.0 ;time after rendering the scan.
	
	scanStart 20 ;left displacement of scan (in px.)
	scanSize 160 ;size of the scan (in px.)
	
	deleteRadius 10.5
	distanceFade 20
	
	Grid {
		size 30;
		divisions 4;
	}
	
	scanStartPoint 12.0;
	
	scanTextStart 50
}

Scenes {
	First {
		textWidth 20
		textHeight 20
		textOffset 20
		linesHeight 30
	}

}

Occlusion {
	bias 0.05;
	radius 40;
	linearAtt 2;
	quadraticAtt 5;
}