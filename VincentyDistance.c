#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "TExaS.h"
#include "uart.h"
#include "PLL.h"
#include "VincentyDistance.h"
#include "UniversalFunctions.h"

#define a 6378137.0 //earth equatorial radius (wgs84 standard)
#define flattening 0.00335281 //flattening factor of ellipsoid (f = 1/298.257223563)
#define b 6356752.3142  //semi-minor axis (b = a * (1-f)
#define ecc 0.08181918275 //eccentricity (e^2 = 2f - f^2)
#define Pi 3.14159265358979323846

// Make modular/Practical Inputs and check if this is working 

double distanceVincenty(double lat1,double lon1,double lat2,double lon2);

double s, lat1, lon1, lat2, lon2, f, U1, U2, L, lambda, lambdaold, itercount, alpha, sigma, C, A, B, u2, cos2sigmam, cossigma, sinsigma, deltasigma; 

//*** Check If Works ***\\\

double distanceVincenty(double lat1,double lon1,double lat2,double lon2){
if (fabs(lat1)>90 | fabs(lat2)>90){
    printf("Input latitudes must be between -90 and 90 degrees, inclusive");
//END CODE - Find way to implement
}

lat1 = lat1 * (Pi/180.0);
lon1 = lon1 * (Pi/180.0);
lat2 = lat2 * (Pi/180.0);
lon2 = lon2 * (Pi/180.0);

if (fabs(Pi/2-fabs(lat1)) < 1e-10){
//  lat1 = sign(lat1)*(pi/2-(1e-10));
    lat1 = lat1*(Pi/2-(1e-10));
}

if ((Pi/2-(lat2)) < 1e-10){
//  lat2 = sign(lat2)*(pi/2-(1e-10));
    lat2 = (lat2)*(Pi/2-(1e-10));
}

f = (a-b)/a;
U1 = atan((1-f)*tan(lat1));
U2 = atan((1-f)*tan(lat2));
lon1 = fmod(lon1,(2*Pi));
lon2 = fmod(lon2,(2*Pi));
L = fabs(lon2-lon1);

if (L > Pi){
    L = 2*Pi - L;
}

//FIX 
lambda = L;
lambdaold = 0;
itercount = 0;

while ((!itercount || fabs(lambda-lambdaold)) > 1e-12){  // force at least one execution
    itercount = itercount+1;
    if (itercount > 50){
        printf("Points are essentially antipodal. Precision may be reduced slightly");
        lambda = Pi;
//End Code? Find a way cross check matlab
				}
    lambdaold = lambda;
    sinsigma = sqrt(pow((cos(U2)*(sin(lambda))),2)+pow((cos(U1)*
        sin(U2)-sin(U1)*cos(U2)*cos(lambda)),2));
    cossigma = sin(U1)*sin(U2)+cos(U1)*cos(U2)*cos(lambda);
    sigma = atan2(sinsigma,cossigma);
    alpha = asin(cos(U1)*cos(U2)*sin(lambda)/sin(sigma));
    cos2sigmam = cos(sigma)-2*sin(U1)*sin(U2)/pow(cos(alpha),2);
    C = f/16*pow(cos(alpha),2)*(4+f*(4-3*pow(cos(alpha),2)));
    lambda = L+(1-C)*f*sin(alpha)*(sigma+C*sin(sigma)*
        (cos2sigmam+C*cos(sigma)*(-1+2*pow(cos2sigmam,2))));
// correct for convergence failure in the case of essentially antipodal points
    if (lambda > Pi){
        printf("Points are essentially antipodal. Precision may be reduced slightly");
        lambda = Pi;


				}
	}
u2 = pow(cos(alpha),2)*(pow(a,2)-pow(b,2))/pow(b,2);
A = 1+u2/16384*(4096+u2*(-768+u2*(320-175*u2)));
B = u2/1024*(256+u2*(-128+u2*(74-47*u2)));
deltasigma = B*sin(sigma)*(cos2sigmam+B/4*(cos(sigma)*(-1+2*pow(cos2sigmam,2))
    -B/6*cos2sigmam*(-3+4*pow(sin(sigma),2))*(-3+4*pow(cos2sigmam,2))));
s = b*A*(sigma-deltasigma);

	printf("\n\n Value of s = %f", s);
	
	return s;
}

void checkExtern(void){
    printf("\n\nCHECK EXTERN = %Lf \n\n", cartesianX);
}


