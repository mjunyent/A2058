#version 330 core
in vec2 UV;
out vec4 color;

void main()
{
	float posx=0.0,posy=0.0;	
	for(int i=0; i<6; i++){
		float unidad=1.0/pow(3.0,float(i+1));
		//if(UV.x>unidad && UV.x<unidad+0.01) color=vec4(1,0,0,1);
		//gif(UV.y>unidad && UV.y<unidad+0.01) color=vec4(1,0,0,1);
		for(float x=posx; x<posx+3*unidad; x+=unidad){
			for(float y=posy; y<posy+3*unidad; y+=unidad){
				if(UV.x>posx+unidad && UV.x<posx+2*unidad && UV.y>posy+unidad && UV.y<posy+2*unidad){
					color = vec4(1,0,0,1);
				}
				if(UV.x>x && UV.x<x+unidad && UV.y>y && UV.y<y+unidad){
					posx=x;
					posy=y;
					//color=vec4(x,y,0,1);
				}
			}
		}

	}
	
	
}


	/*for(int i=0; i<3; i++){
		float unidad=1.0/pow(3.0,float(i));
		for(float j=0; j<1; j+=unidad){
			for(float k=0; k<1; k+=unidad){
				if(UV.x>j+unidad/3 && UV.x<j+2*unidad/3){
					if(UV.y>k+unidad/3 && UV.y<k+2*unidad/3){
						color = vec4(1,0,0,1);
					}
				}

			}
		}

	}*/

	
