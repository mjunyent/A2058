Setup {
	Screen {
		width 1920 ; 1280 ; 2560   ; 1920
		height 1080 ; 720 ; 720  ; 1080
		title Biotechnopolis
		fullscreen NO
	}

	OpenGL {
		Major 4
		Minor 2
	}
}

View {
	Perspective {
		FOV 45.0 ; 25.0
		zNear 3.0
		zFar 700.0
	}
	
	Camera {
		Position 0.0,0.0,10.0
		Direction 0.0,0.0,-1.0
		Up 0.0,1.0,0.0	
	}
	
	Lens {
		FocalLength  50.0
		FocusDistance 200.0
		FStop 2.5
	}
	
	Rig {
		EyeSep 3.0		;2.0
	}
}

Lights {
	number 0;
}

Storm {
	Size 15.0
	blurStrength 0.0
	DOF {
		COCnear 110
		COCfar  200
		radiusNear 80
		radiusFar 40
	}
}

Cells {
	Velocity 0.5	;Depth velocity of cells.
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
		MultMax 2.5
		MultIncr 0.03
		MultDecr 0.01
	}
	
	approachVel 0.02
	
	alphaReduceVel 0.015
	minimumReduceAlpha 0.4
}

Scan {
	box { ; 352.132
		left  -60
		right 50
		up    -25
		down  25
		near  -200 ;-130
		far   -220 ;-140	
	}

	Grid {
		size 3000;
		divisions 4;
	}
	
	restTime 5.0 ;minimum time between end of scan and start of next.
	initialRestTime 5.0;
	startTime 4.0 ;time between cell detected and start of grid draw.

	gridVelocity  1.5 ; 1.0 ;velocity of the grid to move.
	deleteRadius 10.5
	distanceFade 20

	scanStart 20 ;left displacement of scan (in px.)
	scanSize 160 ;size of the scan (in px.)
}

Scenes {
	First {
		textHeight 22
		linesHeight 30
		stillTime 5.0 ;time after rendering the scan.
		zLate 5.0
		World {
			size 25.0
			rotationVel 1.0;
		}
		Polio {
			size 15.0
			rotationVel 1.0;
			color 0.4,0.4,0.4
			velocity 0.1
			AO {
				bias 0.05;
				radius 1;
				linearAtt 1;
				quadraticAtt 1;
			}
		}
	}
	
	Second {
		textHeight 20
		linesHeight 30
		stillTime 5.0

		In {
			size 24.0
			shininess 1.0
			diffuseColor 0.4276,0.9053,0.6196 ;0.83,0.65,0.6196
			specularColor 0.4,0.4,0.4
			rotationVel 0.7
			zoomVel 0.12
		}
		Out {
			size 25.0
			shininess 0.4
			diffuseColor 0.3637,0.4164,0.3637
			specularColor 1.0,1.0,1.0
			opacity 0.5
		}
		AO {
			bias 0.05;
			radius 10;
			linearAtt 2.0;
			quadraticAtt 2;
		}
		Lights {
			number 1
			Light1 {
				type DIRECTIONAL
				attenuation 1.0,0.0,0.0
				direction 0.0,-1.0,0.0
				colour 0.0,1.0,1.0
			}
		}
	}
	
	Third {
		textHeight 15
		linesHeight 30
		stillTime 5.0 ;time after rendering the scan.
		jellyHeight 50.0
		translate -4.0,-4.0,10.0
		displacementVel 0.02;
		displacementVec -1.0,0.8,5.0
	}
	
	Fourth {
		textHeight 15
		linesHeight 30
		stillTime 5.0 ;time after rendering the scan.
		translate -6.0,0.0,10.0
		Fish {
			size 35.0
			rotate 0.0,0.0,-20.0
			rotateVel 0.16
			factor 0.6
			AO {
				bias 0.05;
				radius 10;
				linearAtt 2;
				quadraticAtt 0;
			}
			Lights {
				number 2
					Light1 {
						type DIRECTIONAL
						attenuation 2.0,0.0,0.0
						direction 0.0,-1.0,0.0
						colour 0.5,0.5,1.0
					}
					Light2 {
						type DIRECTIONAL
						attenuation 2.0,0.0,0.0
						direction 1.0,1.0,0.0
						colour 0.6,0.6,1.0
					}
			}
		}
		Eye {
			size 0.9
			position 106.0,-16.0,30.0
		}
	}
	
	Fifth {
		textHeight 15
		linesHeight 25
		stillTime 1.0 ;time after rendering the scan.
		frameHeight 24.9
		frameLate 0.0,0.0,0.0
	}
	
	SixSeventh {
		textHeight 15
		linesHeight 30
		stillTime 5.0
		
		Brain {
			zLate 5.0
			size 25.0
			rotationVel 0.8
			zVel 0.02
			AO {
				bias 0.05;
				radius 20;
				linearAtt 2.0;
				quadraticAtt 10.0;
			}
			Lights {
				number 1
					Light1 {
						type DIRECTIONAL
						attenuation 1.0,0.0,0.0
						direction 0.0,-1.0,0.0
						colour 1.0,1.0,1.0
					}
			}
		}
		
		Skull {
			zLate 10.0
			size 25.0
			teethUpTranslate 0.00,1.28109,1.6
			teethDownTranslate 0.00,-0.60,5.55
			yLate 7.0
			AO {
				bias 0.05;
				radius 5;
				linearAtt 2.0;
				quadraticAtt 2.0;
			}
			Lights {
				number 1
					Light1 {
						type DIRECTIONAL
						attenuation 1.0,0.0,0.0
						direction 0.0,-1.0,0.0
						colour 1.0,1.0,1.0
					}
			}		
		}
	}
	
	Eight {
		textHeight 15
		linesHeight 30
		stillTime 7.0
		Heart {
			zLate 5.0
			height 35.0
			acceleration 0.005
			alphaVel 0.0045
		}
		RBC {
			size 7.0
			shininess 0.4
			diffuseColor 0.8,0.100,0.23
			specularColor 0.1,0.1,0.1
			rotationVel 0.4
			AO {
				bias 0.05;
				radius 20;
				linearAtt 2.0;
				quadraticAtt 2;
			}
			Lights {
				number 1
					Light1 {
						type DIRECTIONAL
						attenuation 1.0,0.0,0.0
						direction 0.0,-1.0,0.0
						colour 1.0,1.0,1.0
					}
			}
			
			disp0 -14.0,5.0,28.0
			disp1 -1.0,-3.0,22.0
			disp2 2.0,4.0,15.0
			disp3 -12.0,2.0,40.0
			disp4 -4.0,-6.0,0.0
			disp5 -10.0,-8.0,26.0
		}
	}

	Ninth {
		textHeight 15
		linesHeight 30
		stillTime 5.0
		Flu {
			size 10.0
			zLate 10.0
			shininess 0.1
			diffuseColor 0.106,0.094,0.235
			specularColor 0.02,0.02,0.02
			rotateVel 0.5
			velocity 0.20

			AO {
				bias 0.05;
				radius 10;
				linearAtt 2.0;
				quadraticAtt 2;
			}
			Lights {
				number 1
					Light1 {
						type DIRECTIONAL
						attenuation 1.0,0.0,0.0
						direction 0.0,-1.0,0.0
						colour 1.0,1.0,1.0
					}
			}
		}
	}
	
	Tenth {
		textHeight 25
		linesHeight 30
		stillTime 9.0
		logoHeight 80
		logoLate 0.5,-3.0,10.0
	}
}

Occlusion {
	bias 0.05;
	radius 40;
	linearAtt 2;
	quadraticAtt 5;
}