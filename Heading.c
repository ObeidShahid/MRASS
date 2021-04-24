#include <stdio.h>
#include <math.h>


#include "Heading.h"

double ilat_rad,ilng_rad,nlat_rad,nlng_rad,Dl,x1,y1,brng,heading_angle_deg;

float Bearing_calc(double i_lat,double i_lng,double n_lat,double n_lng){
	/*printf("\ni_lat = %.5lf",i_lat);
	printf("\ni_lng = %.5lf",i_lng);
	printf("\nn_lat = %.5lf",n_lat);
	printf("\nn_lng = %.5lf",i_lng);*/
    ilat_rad = i_lat *(Pi/180); // convert initial latitude to degrees
    ilng_rad = i_lng *(Pi/180); // convert initial longitude to degrees
	
    nlat_rad = n_lat *(Pi/180); // convert new latitude to degrees
    nlng_rad = n_lng *(Pi/180); // convert new longitude to degrees
	
    // spherical geomtry equations to determine bearing starts //

    Dl = (nlng_rad-ilng_rad);
	
    y1 = cos(ilat_rad)*sin(nlat_rad) - sin(ilat_rad)*cos(nlat_rad)*cos(Dl);

 // //printf("\n\nY= %lf",y1);
	
	x1 = sin(Dl) * cos(nlat_rad);
	////printf("\n\nX= %lf",x1);
	
	brng = atan2(x1,y1);

	//finishes //

	brng = fmod(brng + 2*Pi, 2*Pi); // fmod finds modulus to keep the range of brng calculated between ( -2Pi to 2Pi) so heading is 0-360
	
	//printf("\nbrng= %f",brng);

	// heading angle
	
   if(brng>-1){ // if
        heading_angle_deg = (180 /Pi) * brng;
        //printf("\n\nBearing° = %f\n",heading_angle_deg);
    }else {
        //printf("\n\nstopped moving\n");
        return -1;
        }


    direction_travelled(heading_angle_deg); // will return the direction the reference frame is travelling

	return heading_angle_deg;

}


int direction_travelled(double heading_angle){
    if(heading_angle>-1){
		if(heading_angle ==0 || heading_angle==360){
			printf(" \n\nNorth\n");
			return N_;}//0
			
		else if (heading_angle <=22.5){	
			printf("\n\nNorth North East\n");
			return NNE_;}//1
				
		else if (heading_angle >22.5 && heading_angle <=45){
			printf("\n\nNorth East\n");
			return NE_;}//2
			
		else if (heading_angle >45 && heading_angle <=67.5){
			printf("\n\nEast North East\n");
			return ENE_;}//3	
			
	   else if (heading_angle >67.5 && heading_angle <=90){
			printf("\n\nEast\n");//4
			return E_;}
		
		else if (heading_angle >90 && heading_angle <=112.5){
			printf("\n\nEast South East\n");
			return ESE_;}//5
		
		else if (heading_angle >112.5 && heading_angle <=135){
			printf("\n\nSouth East\n");
			return SE_;}//6
	
		else if (heading_angle >135 && heading_angle <=157.5){
			printf("\n\n South South East\n");
			return SSE_;}//7
		
		else if (heading_angle >157.5 && heading_angle <=180){
			printf("\n\nSouth\n");
			return S_;}//8
		
		else if (heading_angle >180 && heading_angle <=202.5){
			printf("\n\nSouth South West\n");
			return SSW_;}//9
		
		else if (heading_angle >202.5 && heading_angle <=225){
			printf("\n\nSouth West\n");
			return SW_;}//10
		
		else if (heading_angle >225 && heading_angle <=247.5){
			printf("\n\nWest South West\n");
			return WSW_;}//11
		
		else if (heading_angle >247.5 && heading_angle <=270){
			printf("\n\nWest\n");
			return W_;}//12

		else if (heading_angle >270 && heading_angle <=292.5){
			printf("\n\nWest North West\n");
			return WNW_;}//13
		
		else if (heading_angle >292.5 && heading_angle <=315){
			printf("\n\nNorth West\n");
			return NW_;}//14

		else if (heading_angle >315 && heading_angle <=337.5){
			printf("\n\nNorth North West\n");
			return NNW_;}//15
    }
	return 0;
}
/*double _calc(double x, double y){

//// Different cases will have different bearing output 
//	
	if (y==0){
			if(x>0){
				return 0;
			}
			if(x<0){
				return 180;
			}
			if(x==0){
				return -1; // reference frame stopped moving
			}
	}

	if (y<0){
			if(x>0){
				return atan2(x,-y);
			}
			if(x<0){
				return atan2(x,y)-180;
			}
			if(x==0){
				return 270;
			}
	}
//	
	if (y>0){
			if(x>0){
				return atan2(x,y);
			}
			if(x<0){
				return 180-atan2(x,-y);
			}
			if(x==0){
				return 90;
				};
	}

	return 0;
}*/


