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
		zNear 0.1
		zFar 30.0
	}
	
	Camera {
		Position 5.0,0.0,0.0
		Direction -1.0,0.0,0.0
		Up 0.0,1.0,0.0	
	}
	
	Lens {
		FocalLength  0.0
		FocusDistance 60.0
		FStop 1.5
	}
	
	Rig {
		EyeSep 0.0
	}
}

Lights {
	number 3;
	Light1 {
		type POINT
		position 0.0,0.0,0.0
		attenuation 1.0,0.0,0.0
		colour 0.0,0.0,0.0	
	}
	
	Light2 {
		type DIRECTIONAL
		attenuation 2.0,10000.0,0.0
		direction 0.0,0.0,1.0
		colour 1.0,1.0,1.0
	}
	
	Light3 {
		type DIRECTIONAL
		attenuation 2.0,0.0,0.0
		direction 0.0,0.0,-1.0
		colour 0.0,0.0,1.0	
	}

;	Light11 {
;		type DIRECTIONAL
;		attenuation 1.0,0.0,0.0
;		direction 1.0,0.0,0.0
;		colour 1.0,0.0,0.0
;	}
;	Light12 {
;		type POINT
;		attenuation 1.0,0.0,0.0
;		position 0.0,0.0,0.0
;		colour 0.0,1.0,0.0
;	}
;	Light13 {
;		type SPOTLIGHT
;		attenuation 1.0,0.0,0.0
;		position 0.0,0.0,0.0
;		colour 0.0,1.0,0.0
;		direction 1.0,0.0,0.0
;		outerCutoff 0.96
;		innerCutoff 0.8
;		exponent 0.512
;	}
}

TestSphere {
	Position 0.0,0.0,92.0
}

Spheres {
	RenderBox 0.0 ; if bigger than one render, else, doesnt.
	Velocity 0.5
	zMax 100.0
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