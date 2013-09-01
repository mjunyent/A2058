; Simple cells config file

View {
	Perspective {
		FOV 45.0;
		zNear 5.0;
		zFar 28.0;
	}
	
	Camera {
		Position 0.0,-6.0,14.0;
		Direction 0.0,-4.0,-14.0;
		Up 0.0,1.0,0.0;		
	}
	
	Lens {
		FocalLength 0.0;
		FocusDistance 15.0;
		FStop 0.5;
	}
	
	Rig {
		EyeSep 0.2;
	}
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