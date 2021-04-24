// Class used to determine the direction the reference frame is heading
#ifndef Heading_h
#define Heading_h
#define Pi 3.14159265358979323846
typedef enum {
	N_,   //North                0°/360°  ---> 0
	NNE_, // North North east    022.5°   ---> 1
	NE_,  //North East           045°     ---> 2
	ENE_, //East north East      067.5°   ---> 3
	E_,   // East                090°     ---> 4
	ESE_, //East south east      112.5°   ---> 5
	SE_,  //South east           135 °    ---> 6
    SSE_, // South south east    157.5°   ---> 7
    S_,   // south               180°     ---> 8
    SSW_, //South south west     202.5°   ---> 9
    SW_,  //South west           225°     ---> 10
    WSW_, // West South West     247.5°   ---> 11
	W_,   // West                270°     ---> 12
	WNW_, // West North West     292.5°   ---> 13
	NW_,  // North West          315°     ---> 14
	NNW_  // North North West    337.5°   ---> 15
}Direction_travelled;


float Bearing_calc(double i_lat,double i_lng,double n_lat,double n_lng);
double _calc(double x, double y);
int direction_travelled(double heading_angle);

#endif
