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
		EyeSep 2.0 ;2.0
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
	centerPosition 55.0,0.0,-145.0
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
	
	approachVel 0.02
}

Scan {
	box {
		left  -60
		right 50
		up    -25
		down  25
		near  -200 ;-130
		far   -220 ;-140	
	}

	Grid {
		size 30;
		divisions 4;
	}
	
	restTime 4.0 ;minimum time between end of scan and start of next.
	startTime 2.0 ;time between cell detected and start of grid draw.

	gridVelocity  1.5 ; 1.0 ;velocity of the grid to move.
	deleteRadius 10.5
	distanceFade 20

	scanStart 20 ;left displacement of scan (in px.)
	scanSize 160 ;size of the scan (in px.)
}

Scenes {
	First {
		textHeight 20
		linesHeight 30
		stillTime 5.0 ;time after rendering the scan.
		zLate 5.0
		World {
			size 25.0
		}
		Polio {
			size 25.0
			AO {
				bias 0.05;
				radius 2;
				linearAtt 1;
				quadraticAtt 1;
			}
		}
	}
	
	Fourth {
		textHeight 15
		linesHeight 30
		stillTime 5.0 ;time after rendering the scan.
		translate -12.0,5.0,10.0
		Fish {
			size 35.0
			rotate -10.0,-20.0,-50.0
			AO {
				bias 0.05;
				radius 5;
				linearAtt 1;
				quadraticAtt 1;
			}
			Lights {
				number 1
					Light1 {
						type DIRECTIONAL
						attenuation 2.0,0.0,0.0
						direction -1.0,1.0,1.0
						colour 0.5,0.5,1.0
					}
			}
		}
		Eye {
			size 0.9
			position 106.0,-16.0,35.0
		}
	
	}
}

Occlusion {
	bias 0.05;
	radius 40;
	linearAtt 2;
	quadraticAtt 5;
}